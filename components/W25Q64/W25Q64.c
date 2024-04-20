#include "stm32f10x.h"
#include "coustomSPI.h"
#include "W25Q64_Ins.h"

/**
 * 初始化
 *  拼接指令集时序
 *
 */
void W25Q64_Init(void){
    customSPI_Init();
}

/**
 *
 * 获取 ID
 * 连续拼接获取三个 时序
 *  获取
 * @param MID   厂商 ID
 * @param DID   设备 ID
 *
 *
 * 1、 start
 * 2、交换发送一个字节  0x9F, 发送指令到 W25Q64
 * 3、W25Q64 收到指令后 ，想主机发送 字节，之际 通过 0xFF 将数据交换过来
 * 4、接收 以时序传过来的数据
 * 5、在交换一次 就时设备 id 的高 8 位
 * 6、 *DID 左移 8位
 * 7、再交换一次 就时设备 id 的低 8 位
 */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID){
    //uint8_t ByteReceive;
    customSPI_Start();
    // 交换发送一个字节 W25Q64_JEDEC_ID 0x9F  , 发送指令到 W25Q64
    customSPI_SwapByte(W25Q64_JEDEC_ID);
    // W25Q64 收到指令后 ，想主机发送 字节，之际 通过 W25Q64_DUMMY_BYTE 0xFF 将数据交换过来； 0xFF 没有任何意义
    *MID = customSPI_SwapByte(W25Q64_DUMMY_BYTE);
    // 接收 以时序传过来的数据
    // 在交换一次 就时设备 id 的高 8 位
    *DID = customSPI_SwapByte(W25Q64_DUMMY_BYTE);
    // *DID 左移 8位
    *DID <<= 8;
    // 再交换一次 就时设备 id 的低 8 位
    *DID = *DID | customSPI_SwapByte(W25Q64_DUMMY_BYTE);
    // 停止
    customSPI_Stop();
}


/**
* 写使能
 * 发送 0x06
 *
 * 每次 页编程 W25Q64_PageProgram 写入前 都需要进入到 写 使能的状态， 每次 写使能 只对 当次 有效
 * 擦除数据 也是 写入数据 所以也要写 使能
*/
void W25Q64_WriteEnable(void){
    customSPI_Start();
    customSPI_SwapByte(W25Q64_WRITE_ENABLE);
    customSPI_Stop();
}

/**
 * 读状态寄存器 1
 * 发送指令码 0x05，发送完成就可以读 状态寄存器 了
 * 读 状态寄存器 的主要用途 救赎判断 芯片是不熟 忙 状态
 *  读取状态 位 busy ： 1表示 忙 ；0表示 不忙
 */
// void W25Q64_ReadStatusRegister(){
// }
 // 等待 busy 为 0
 //  读状态寄存器 1
 /**
  * 等待 busy 为 0
  * 读 状态寄存器1
  *
  */
void W25Q64_WaitBusy(void){
     uint32_t timeOut;
     customSPI_Start();
     // 读状态寄存器1
     customSPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
     timeOut = 10000;
     // 交换一个字节 接收数据; 取出 最低位
     // 判断 最低位 是否位1
     while ((customSPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01){
         timeOut--;
         if(timeOut == 0){
             break;
         }
     };
     customSPI_Stop();
}

/**
 * 页编程  W25Q64_PAGE_PROGRAM
 * （一次 最大256 个字节）
 *
 * @param Address       地址
 * @param *DataArry     要传输数据的指针； 每一位是 一个字节
 * @param Count         计数  要用 uint16_t ，如果使用 uint8_t   只能发送 0～255 个数； 在32位系统中一个char类型一般为8个bit，所以能存储的数据范围为-128~127，而unsigned char则是0~255
 *
 * 1、start
 * 2、发送 W25Q64_PAGE_PROGRAM 开启页编程
 * 3、交换发送 3 个字节的 地址
 * 4、发送数据
 * 5、每次写入操作之后，芯片进入 忙状态，所以调用一下W25Q64_WaitBusy
 */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArry, uint16_t Count){
    uint16_t i;
    W25Q64_WriteEnable();//写使能
    customSPI_Start();
    customSPI_SwapByte(W25Q64_PAGE_PROGRAM);
    customSPI_SwapByte(Address >> 16); // 23:16 位
    customSPI_SwapByte(Address >> 8);// 15: 8   位
    customSPI_SwapByte(Address);// 7:0          位
    for (i = 0; i < Count; ++i) {
        customSPI_SwapByte(DataArry[i]);
    }
    customSPI_Stop();

    W25Q64_WaitBusy();
}


/**
 * 擦除的功能
 * 扇区擦除 W25Q64_SECTOR_ERASE_4KB
 * 写使能
 * 发送指令 0x20 再发送 3个字节的地址
 * 这样指定地址的整个扇区都会被擦除
 *
 * 每次写入操作之后，芯片进入 忙状态，所以调用一下W25Q64_WaitBusy
 */
void W25Q64_SectorErase(uint32_t Address){
    W25Q64_WriteEnable();//写使能
    customSPI_Start();
    customSPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    customSPI_SwapByte(Address >> 16); // 23:16 位
    customSPI_SwapByte(Address >> 8);// 15: 8   位
    customSPI_SwapByte(Address);// 7:0          位
    customSPI_Stop();

    W25Q64_WaitBusy();
}

/**
 * 连续接收数据 可以一直读取    W25Q64_READ_DATA 0x03
 * @param Address   读取数据的地址
 * @param DataArry  读取出来的数据
 * @param Count     读取次数
 *
 *
 */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArry, uint32_t Count){

    uint32_t i;
    customSPI_Start();
    customSPI_SwapByte(W25Q64_READ_DATA);
    customSPI_SwapByte(Address >> 16); // 23:16 位
    customSPI_SwapByte(Address >> 8);// 15: 8   位
    customSPI_SwapByte(Address);// 7:0          位
    for (i = 0; i < Count; ++i) {
        DataArry[i] = customSPI_SwapByte(W25Q64_DUMMY_BYTE);
    }
    customSPI_Stop();
}

