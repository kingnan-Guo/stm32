#include "stm32f10x.h"

#define SPI_SCK     GPIO_Pin_5  // SCK      SPI 时钟              是 硬件控制的 所以配置 复用推挽输出
#define SPI_D0      GPIO_Pin_6  // MISO     SPI主机输入从机输出     硬件外设输入信号 上拉输入； 普通GPIO 可以输入，外设也可以输入
#define SPI_D1      GPIO_Pin_7  // MOSI     SPI主机输出从机输入     是 硬件控制的 所以配置 复用推挽输出
#define SPI_CS      GPIO_Pin_4  // SS       SPI 片选              软件控制的 控制信号 ，所以 通用推挽输出


//控制
void customSPI_W_SS(uint8_t BitValue){
    GPIO_WriteBit(GPIOA, SPI_CS, (BitAction)BitValue);
}
////输出
//void customSPI_W_MISO(uint8_t BitValue){
//    GPIO_WriteBit(GPIOA, SPI_D0, (BitAction)BitValue);
//}
////时钟
//void customSPI_W_SCK(uint8_t BitValue){
//    GPIO_WriteBit(GPIOA, SPI_SCK, (BitAction)BitValue);
//}
////输入
//uint8_t customSPI_R_MOSI(void){
//    // 读取 GPIOA 的 GPIO_Pin_7 值返回
//    return GPIO_ReadInputDataBit(GPIOA, SPI_D1);
//}
/**
 * 初始化 SPI 通信引脚
 * 1、开始时钟
 * 2、开启SPI1 的外设时钟
 * 3、初始化 引脚
 * 4、初始化 SPI 外设
 *
 *
 * =============
 * SCK 、MOSI 是 硬件控制的 所以配置 复用推挽输出
 * MISO 硬件外设输入信号 上拉输入
 * SS 通用 推挽输出
 *
 * SPI 选参数 使用结构体配置即可
 *
 * 最后 SPI_cmd()
 */
void customSPI_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 开启SPI1 的外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    // 初始化 SPI_CS 引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 通用 推挽输出
    GPIO_InitStructure.GPIO_Pin = SPI_CS;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // 初始化 SCK MOSI 引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用 推挽输出
    GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_D1;// SCK MOSI
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // 初始化 MISO 引脚  上拉输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入 GPIO_Mode_IPU  或 浮空输入
    GPIO_InitStructure.GPIO_Pin = SPI_D0;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // 初始化 SPI 外设
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//    选择SPI模式 STM32 主机
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//标准模式 双线全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位 ； 发送 8 位还是 16 位 数据帧
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位； 高位先行还是低位先行
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;// 72MHZ /128 ； 波特率预分频器
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//第一个边沿采样（移入）  0模式
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 时钟相位 0模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// NSS 用于软件 模拟
    SPI_InitStructure.SPI_CRCPolynomial = 7;// CRC校验模式
    //SPI_InitStructure = ;


    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);


    //
    customSPI_W_SS(1);

}

/**
 * 起始
 * 1、先将SS置低电平
 * 2、
 */
void customSPI_Start(void){
    customSPI_W_SS(1);


}

/**
 * 终止
 * 将SS置低电平
 */
void customSPI_Stop(void){
    customSPI_W_SS(0);
}

/**
 * 交换一个字节
 *
 * 1、检测 SPI1 的 TXE ; TDR 是否为 空
 *
 */
uint8_t customSPI_SwapByte(uint8_t ByteSend){
    uint8_t ByteReceive;
    // 检测 SPI1 的 TXE    SPI_I2S_FLAG_TXE: Transmit buffer empty flag
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
    // 软件写入数据到 TDR ； 写入数据 顺便清楚 TXE 标志位
    SPI_I2S_SendData(SPI1, ByteSend);
    //byteSend 自动转入到 移位寄存器 ； 波形自动产生

    // 接收移位完成时 会 置 标志位 RXNE 1；检测 SPI1 的 RXNE ； SPI_I2S_FLAG_RXNE: Receive buffer not empty flag.
    // 读取数据 顺便清楚 RXNE 标志位
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);

    // 读取DR
    ByteReceive = SPI_I2S_ReceiveData(SPI1);

    return ByteReceive;


}



