//
// Created by 90175 on 2024/11/20.
//
#include "stm32f10x.h"
#include "SOFTWARE_SPI.h"
#include "SOFTWARE_CONTROL_W25Q64.h"
#include "W25Q64_Ins.h"
#include "OLED.h"


void SOFTWARE_CONTROL_W25Q64_INIT(){
    SOFTWARE_SPI_INIT();
}

// 写使能; 每一次 页编程 都需要进行一次 开启写使能
void SOFTWARE_CONTROL_W25Q64_WRITE_ENABLE(){
    SOFTWARE_SPI_START();
    SOFTWARE_SPI_SWAPBYTE(W25Q64_WRITE_ENABLE);
    SOFTWARE_SPI_STOP();
}

// 查看 BUSY 标志位
void SOFTWARE_CONTROL_W25Q64_WAIT_BUSY(){
    uint32_t TimeOut;
    SOFTWARE_SPI_START();


    SOFTWARE_SPI_SWAPBYTE(W25Q64_READ_STATUS_REGISTER_1); // 读状态寄存器1  ; 发送指令后 开始读取； BUSY 在最低位
    TimeOut = 10000;
    // 读取 Busy 位； 使用 FF 交换
    while ((SOFTWARE_SPI_SWAPBYTE(W25Q64_DUMMY_BYTE) & 0x01) == 0x01){
        TimeOut--;
        if(TimeOut == 0){
            break;
        }
    };
    SOFTWARE_SPI_STOP();
}


// 去读取数据， 用FF 交换
void SOFTWARE_CONTROL_W25Q64_READ_DATA(uint32_t Address, uint8_t * DATA_ARRAY,uint32_t Count){
    uint32_t i;
    SOFTWARE_SPI_START();

    SOFTWARE_SPI_SWAPBYTE(W25Q64_READ_DATA);// 读指令

    // 读地址
    SOFTWARE_SPI_SWAPBYTE(Address >> 16);// 最高的 八位
    SOFTWARE_SPI_SWAPBYTE(Address >> 8);// 中间 八位
    SOFTWARE_SPI_SWAPBYTE(Address);// 低 八位
    for ( i = 0; i < Count; i++) {
        // 使用 0xFF 去交换数据
        DATA_ARRAY[i] = SOFTWARE_SPI_SWAPBYTE(W25Q64_DUMMY_BYTE);
    }


    SOFTWARE_SPI_STOP();


}

// 页编程
// 这里 要传 指针，也就是  ARRAY 数据的 第一个数据的 指针地址
void SOFTWARE_CONTROL_W25Q64_PAGE_PROGRAM(uint32_t Address, uint8_t * DATA_ARRAY,uint32_t Count){
    SOFTWARE_CONTROL_W25Q64_WRITE_ENABLE();// 写使能
    SOFTWARE_SPI_START();

    SOFTWARE_SPI_SWAPBYTE(W25Q64_PAGE_PROGRAM);// 写 指令


    SOFTWARE_SPI_SWAPBYTE(Address >> 16);// 最高的 八位
    SOFTWARE_SPI_SWAPBYTE(Address >> 8);// 中间 八位
    SOFTWARE_SPI_SWAPBYTE(Address);// 低 八位

    for (int i = 0; i < Count; ++i) {
        SOFTWARE_SPI_SWAPBYTE(DATA_ARRAY[i]);
    }


    SOFTWARE_SPI_STOP();
    SOFTWARE_CONTROL_W25Q64_WAIT_BUSY();// 等待 BUSY
}

// 擦除的功能
// 扇区  擦除，因为扇区  是4K
// 一次性接受 32 位 地址后，分三次发送给 W25Q64
void SOFTWARE_CONTROL_W25Q64_SECTOR_ERASE(uint32_t Address){
    SOFTWARE_CONTROL_W25Q64_WRITE_ENABLE();// 写使能
    SOFTWARE_SPI_START();

    SOFTWARE_SPI_SWAPBYTE(W25Q64_SECTOR_ERASE_4KB);

    SOFTWARE_SPI_SWAPBYTE(Address >> 16);// 最高的 八位
    SOFTWARE_SPI_SWAPBYTE(Address >> 8);// 中间 八位
    SOFTWARE_SPI_SWAPBYTE(Address);// 低 八位


    SOFTWARE_SPI_STOP();

    SOFTWARE_CONTROL_W25Q64_WAIT_BUSY();// 等待 BUSY 大概 30ms
}


// 读取 ID
void SOFTWARE_CONTROL_W25Q64_READ_ID(uint8_t *MID, uint16_t *DID){
    SOFTWARE_CONTROL_W25Q64_WRITE_ENABLE();// 写使能
    SOFTWARE_SPI_START();

    // 发送指令
    SOFTWARE_SPI_SWAPBYTE(W25Q64_JEDEC_ID);
    // 读取
    // W25Q64 收到指令后 ，想主机发送 字节，之际 通过 W25Q64_DUMMY_BYTE 0xFF 将数据交换过来； 0xFF 没有任何意义
    *MID = SOFTWARE_SPI_SWAPBYTE(W25Q64_DUMMY_BYTE);
    // 接收 以时序传过来的数据
    // 在交换一次 就时设备 id 的高 8 位
    *DID = SOFTWARE_SPI_SWAPBYTE(W25Q64_DUMMY_BYTE);
    // *DID 左移 8位
    *DID <<= 8;
    // 再交换一次 就时设备 id 的低 8 位
    *DID = *DID | SOFTWARE_SPI_SWAPBYTE(W25Q64_DUMMY_BYTE);


    SOFTWARE_SPI_STOP();
    SOFTWARE_CONTROL_W25Q64_WAIT_BUSY();// 等待 BUSY
}




void SOFTWARE_CONTROL_W25Q64_MAIN(){

    uint8_t MID;
    uint16_t DID;
    uint8_t dataArray_write[] = {0x01, 0x03, 0x05,0x07};
    uint8_t dataArr_Read[4];

    uint32_t startAddr = 0x000000;

    SOFTWARE_CONTROL_W25Q64_INIT();

    // 读 ID =======
    SOFTWARE_CONTROL_W25Q64_READ_ID(&MID, &DID);
    OLED_ShowString(1, 1, "MID ");
    OLED_ShowHexNum(1, 5, MID, 2);
    OLED_ShowString(2, 1, "DID ");
    OLED_ShowHexNum(2, 5, DID, 4);




    // 读写数据  =====

    OLED_ShowString(3, 1, "W: ");
    OLED_ShowString(4, 1, "R: ");

    // 写入前先擦除
    SOFTWARE_CONTROL_W25Q64_SECTOR_ERASE(startAddr);
    // 写入数据
    SOFTWARE_CONTROL_W25Q64_PAGE_PROGRAM(startAddr, dataArray_write, 4);
    // 读取数据
    SOFTWARE_CONTROL_W25Q64_READ_DATA(startAddr, dataArr_Read, 4);


    OLED_ShowHexNum(3, 3, dataArray_write[0], 2);
    OLED_ShowHexNum(3, 6, dataArray_write[1], 2);
    OLED_ShowHexNum(3, 9, dataArray_write[2], 2);
    OLED_ShowHexNum(3, 12, dataArray_write[3], 2);

    OLED_ShowHexNum(4, 3, dataArr_Read[0], 2);
    OLED_ShowHexNum(4, 6, dataArr_Read[1], 2);
    OLED_ShowHexNum(4, 9, dataArr_Read[2], 2);
    OLED_ShowHexNum(4, 12, dataArr_Read[3], 2);

}