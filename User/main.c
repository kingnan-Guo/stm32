#include "stm32f10x.h"
#include "OLED.h"
//#include "delay.h"
#include "W25Q64.h"

uint8_t MID;
uint16_t DID;

uint8_t dataArray_write[] = {0x01, 0x02, 0x03,0x05};
uint8_t dataArr_Read[4];

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
    W25Q64_Init();
    W25Q64_ReadID(&MID, &DID);
//    OLED_ShowString(1, 1, "start ");
    OLED_ShowString(1, 1, "MID ");
    OLED_ShowHexNum(1, 5, MID, 2);
    OLED_ShowString(2, 1, "DID ");
    OLED_ShowHexNum(2, 5, DID, 4);


    OLED_ShowString(3, 1, "W: ");
    OLED_ShowString(4, 1, "R: ");

    /*
     * 写入测试
     * 1、写入前 先擦除
     *      一般指定扇区起始地址来擦除
     *      Address 0x000000； 最后三位 不管如何变化， 都是 在同一个扇区，比如说 0x000000 与 0x0000FF 擦除的是统同一个扇区
     *      Address 0x000000   前四个 0 是 页地址； 后两个 0 是 页内地址
     *  2、开始写入数据
     *  3、写入数据 再读取
     */
    // 先擦除
    W25Q64_SectirErase(0x000000);
    // 写入数据
    W25Q64_PageProgram(0x000000, dataArray_write, 4);
    // 写入数据 再读取
    W25Q64_ReadData(0x000000, dataArr_Read, 4);
    OLED_ShowHexNum(3, 3, dataArray_write[0], 2);
    OLED_ShowHexNum(3, 6, dataArray_write[1], 2);
    OLED_ShowHexNum(3, 9, dataArray_write[2], 2);
    OLED_ShowHexNum(3, 12, dataArray_write[3], 2);

    OLED_ShowHexNum(4, 3, dataArr_Read[0], 2);
    OLED_ShowHexNum(4, 6, dataArr_Read[1], 2);
    OLED_ShowHexNum(4, 9, dataArr_Read[2], 2);
    OLED_ShowHexNum(4, 12, dataArr_Read[3], 2);

    while(1) {

    }
}



/**
 *
 *
 *
 */

