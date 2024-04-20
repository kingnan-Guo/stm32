#include "stm32f10x.h"

#define SPI_SCK     GPIO_Pin_5  // SCK      SPI 时钟
#define SPI_D0      GPIO_Pin_6  // MISO     SPI主机输入从机输出
#define SPI_D1      GPIO_Pin_7  // MOSI     SPI主机输出从机输入
#define SPI_CS      GPIO_Pin_4  // SS       SPI 片选


//控制
void customSPI_W_SS(uint8_t BitValue){
    GPIO_WriteBit(GPIOA, SPI_CS, (BitAction)BitValue);
}

//时钟
void customSPI_W_SCK(uint8_t BitValue){
    GPIO_WriteBit(GPIOA, SPI_SCK, (BitAction)BitValue);
}

// SPI主机输入从机输出 MySPI_R_MISO
uint8_t customSPI_R_MISO(void){
    return GPIO_ReadInputDataBit(GPIOA, SPI_D0);
}

// SPI主机输出从机输入
void customSPI_W_MOSI(uint8_t BitValue){
    // 读取 GPIOA 的 GPIO_Pin_7 值返回
    GPIO_WriteBit(GPIOA, SPI_D1, (BitAction)BitValue);
}
/**
 * 初始化 SPI 通信引脚
 * 1、开始时钟
 * 2、初始化引脚 PA6
 */
void customSPI_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化 引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 推挽输出
    GPIO_InitStructure.GPIO_Pin = SPI_SCK | SPI_D1 | SPI_CS;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA,&GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入 GPIO_Mode_IPU  或 浮空输入
    GPIO_InitStructure.GPIO_Pin = SPI_D0;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // 初始化引脚之后 置 引脚的默认电平
    // 最常用的时模式 0 ；
    // CPOL = 0；空闲状态 SCK 为 低电平； CPHA = 0
    // CPHA=0: SCK第一个边沿移入数据「主机从机同时移入数据； 主机 MOSI 进行采样，输入数据、从机 MOSI 进行采样 ，输入数据；但是在最开始还没有移出如何移入，处理的办法是 向前提前半个相位，在SCK 还未变成高电平的时候就开始移出数据『SS 下降沿 会触发 ，SS 下降沿 这时 SCk 还是低电平，所以直接开始移出数据』，然后到了第一个上升沿，开始移入数据；」，
    // 第二个边沿 移出数据「主机从机同时移出数据 ；主机MOSI 输出， 从机MISO 输出 」；

    customSPI_W_SS(1);
    customSPI_W_SCK(0);

}

/**
 * 起始
 * 1、先将SS置低电平
 * 2、
 */
void customSPI_Start(void){
    customSPI_W_SS(0);


}

/**
 * 终止
 * 将SS置低电平
 */
void customSPI_Stop(void){
    customSPI_W_SS(1);
}

/**
 * 交换一个字节
 *
 *
 */
uint8_t customSPI_SwapByte(uint8_t ByteSend){
    uint8_t ByteReceive = 0x00;
    uint8_t i;

    for (i = 0; i < 8; i ++) {

        // 主机移出数据 数据的最高位放放到 MOSI 上 ; 从机移出数据 将数据放到 MISO 上
        customSPI_W_MOSI(ByteSend & (0x80 >> i));
        // SCK 开始上升沿
        customSPI_W_SCK(1);
        //上升沿时 主机 将 放在 MOSI 上的 1 位数据 读出来，放到移位寄存器的最低位； 从机  写入一位数据
        // 这时 理论上要等待一段时间，但是没有标志位，所以判断 是否 有 MISO 的数据
        if (customSPI_R_MISO() == 1){
            // 接收到了数据
            ByteReceive |= (0x80 >> i);
        }
        //SCK 下降沿
        customSPI_W_SCK(0);
        // 主机从机 开始移出 下移位 循环
    }




    return ByteReceive;


}



uint8_t customSPI_SwapByte2(uint8_t ByteSend)
{
    uint8_t i, ByteReceive = 0x00;

    for (i = 0; i < 8; i ++)
    {
        customSPI_W_MOSI(ByteSend & (0x80 >> i));
        customSPI_W_SCK(1);
        if (customSPI_R_MISO() == 1){
            // 接收到了数据
            ByteReceive |= (0x80 >> i);
        }
        customSPI_W_SCK(0);
    }

    return ByteReceive;
}



/**
 * 第二种方法 移位进行数据交换，效率更高； 但是原始数据被改变
 *
 * @param ByteSend
 * @return
 */
uint8_t customSPI_SwapByte_2(uint8_t ByteSend){
    uint8_t ByteReceive;
    uint8_t i;

    for (i = 0; i < 8; i++) {

        // 主机移出数据 数据的最高位放放到 MOSI 上 ; 从机移出数据 将数据放到 MISO 上
        customSPI_W_MOSI(ByteSend & 0x80);
        ByteSend <<= 1;
        // SCK 开始上升沿
        customSPI_W_SCK(1);
        //上升沿时 主机 将 放在 MOSI 上的 1 位数据 读出来，放到移位寄存器的最低位； 从机  写入一位数据
        // 这时 理论上要等待一段时间，但是没有标志位，所以判断 是否 有 MISO 的数据
        if(customSPI_R_MISO() == 1){
            // 接收到了数据
            ByteSend |= 0x01;
        }
        //SCK 下降沿
        customSPI_W_SCK(0);
        // 主机从机 开始移出 下移位 循环
    }
    return ByteSend;


}