//
// Created by 90175 on 2024/11/20.
//
#include "stm32f10x.h"
#include "HARDWARE_SPI.h"

#define SPI_PORT    GPIOA
#define SPI_SCK     GPIO_Pin_5  // SCK      SPI 时钟              是 硬件控制的 所以配置 复用推挽输出
#define SPI_D0      GPIO_Pin_6  // MISO     SPI主机输入从机输出     硬件外设输入信号 上拉输入； 普通GPIO 可以输入，外设也可以输入
#define SPI_D1      GPIO_Pin_7  // MOSI     SPI主机输出从机输入     是 硬件控制的 所以配置 复用推挽输出
#define SPI_CS      GPIO_Pin_4  // SS       SPI 片选              软件控制的 控制信号 ，所以 通用推挽输出



void HARDWARE_SPI_W_CS(uint8_t BitValue){
    GPIO_WriteBit(SPI_PORT, SPI_CS, (BitAction)BitValue);
}

void HARDWARE_SPI_INIT(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    // CS SS 独立 GPIO 控制
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SPI_CS;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);


    // 初始化 SCK 和 MOSI 复用 推挽 输出 ，直接 连接 外设
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_D1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);


    // 主机输入到从机 要有 一定的 驱动能力;  初始化 MISO 引脚  上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = SPI_D0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_PORT, &GPIO_InitStructure);



    // 开启 SPI1 外设
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);


    // 初始化 SPI 外设
    SPI_InitTypeDef SPI_InitStruct;
    SPI_StructInit(&SPI_InitStruct);
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;// 作为 主机
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 标准模式 双线全双工
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;// 发送 8 位 数据帧
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// 高位先行
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//  SPI_BaudRatePrescaler_2  分频
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;// 指定捕获时钟  边沿； 捕获 1 个边沿
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;// 时钟相位 0 模式
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;// 这个 暂时不用
    SPI_InitStruct.SPI_CRCPolynomial = 7;//CRC 校验模式

    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);


    HARDWARE_SPI_W_CS(1);
}

void HARDWARE_SPI_START(){
    HARDWARE_SPI_W_CS(0);
}

void HARDWARE_SPI_STOP(){
    HARDWARE_SPI_W_CS(1);
}

// 发送数据
// CS 拉低
// 先放置数据 然后
uint8_t HARDWARE_SPI_SWAPBYTE(uint8_t BYTE){
    uint8_t RECEIVE_BYTE = 0x00;

    // 检测 TXE 移位寄存器 为 空 标志位；为空就下写入 数据
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1, BYTE);


    // RXNE 接收寄存器 不为空, 读取数据
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
    RECEIVE_BYTE = SPI_I2S_ReceiveData(SPI1);

    return RECEIVE_BYTE;
}

