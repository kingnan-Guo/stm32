//
// Created by 90175 on 2024/11/20.
//
#include "stm32f10x.h"
#include "SOFTWARE_SPI.h"

// 0 模式
#define SPI_PORT    GPIOA
#define SPI_SCK     GPIO_Pin_5  // SCK      SPI 时钟              是 硬件控制的 所以配置 复用推挽输出
#define SPI_D0      GPIO_Pin_6  // MISO     SPI主机输入从机输出     硬件外设输入信号 上拉输入； 普通GPIO 可以输入，外设也可以输入
#define SPI_D1      GPIO_Pin_7  // MOSI     SPI主机输出从机输入     是 硬件控制的 所以配置 复用推挽输出
#define SPI_CS      GPIO_Pin_4  // SS       SPI 片选              软件控制的 控制信号 ，所以 通用推挽输出

void SOFTWARE_SPI_W_CS(uint8_t BitValue){
    GPIO_WriteBit(SPI_PORT, SPI_CS, (BitAction)BitValue);
}

void SOFTWARE_SPI_INIT(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    // 开漏输出 有输入能力
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SPI_CS | SPI_SCK | SPI_D1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);

    // 主机输入到从机 要有 一定的 驱动能力
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = SPI_D0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);

    SOFTWARE_SPI_W_CS(1);

    SOFTWARE_SPI_W_CS(0);
}




void SOFTWARE_SPI_W_SCK(uint8_t BitValue){
    GPIO_WriteBit(SPI_PORT, SPI_SCK, (BitAction)BitValue);
}

uint8_t SOFTWARE_SPI_R_MISO(){
    return GPIO_ReadInputDataBit(SPI_PORT, SPI_D0);
}

void SOFTWARE_SPI_W_MOSI(uint8_t BitValue){
    GPIO_WriteBit(SPI_PORT, SPI_D1, (BitAction)BitValue);
}

void SOFTWARE_SPI_START(){
    SOFTWARE_SPI_W_CS(0);
}

void SOFTWARE_SPI_STOP(){
    SOFTWARE_SPI_W_CS(1);
}

// 发送数据
// CS 拉低
// 先放置数据 然后
uint8_t SOFTWARE_SPI_SWAPBYTE(uint8_t BYTE){
    uint8_t i;
    uint8_t RECEIVE_BYTE = 0x00;
    for ( i = 0; i < 8; i++) {
        SOFTWARE_SPI_W_MOSI(BYTE & (0x80 >> i));
        SOFTWARE_SPI_W_SCK(1);// 上升沿发送数据
        if(SOFTWARE_SPI_R_MISO() == 1){
            RECEIVE_BYTE |= (0x80 >> i);
        }
        SOFTWARE_SPI_W_SCK(0);// 时钟 下降沿 采集
    }
    return RECEIVE_BYTE;
}


