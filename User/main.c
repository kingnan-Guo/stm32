#include "stm32f10x.h"

#include "OLED.h"
#include "delay.h"
#include "customI2C.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();

    customI2C_Init();
    customI2C_Start();
    customI2C_SendByte(0xD2 );// 1101 0000 写入操作
    // 发送一个数据后要接收 一个应答位
    uint8_t ACK = customI2C_ReceiveAckByte();
    customI2C_Stop();
    OLED_ShowNum(1, 1, ACK, 8);
    while(1) {

    }
}


