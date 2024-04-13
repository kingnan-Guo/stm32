#include "stm32f10x.h"
#include "delay.h"

// 端口 换名字 宏定义
#define SCL_PORT    GPIOC
#define SCL_PIN     GPIO_Pin_15
#define SDA_PIN     GPIO_Pin_14



/**
 * 初始化 I2C
 * 1、初始化GPIO
 *
 *
 * 其实我想 外部传值初始化哪个引脚， 而不是里面写固定的
 */
void customI2C_Init(void){

    // 创建时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;// 开漏输出 ； 开漏输出技能输出 也能输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 初始化完成后 STM32 主机 释放总线  GPIO_Pin_15 GPIO_Pin_14； 此时 SCL SDA 处于 高电平状态，I2C总线处于 空闲状态
    GPIO_SetBits(GPIOC, GPIO_Pin_15 | GPIO_Pin_14 );


}

// 封装函数 用于拉低抬高电平
void customI2C_W_SCL(uint8_t BitValue){
    GPIO_WriteBit(SCL_PORT, SCL_PIN, (BitAction)BitValue);// (BitAction)BitValue 非 0 及 1
    Delay_ms(10);// 延迟10us 是为了可以让数据正确接收到
}

void customI2C_W_SDA(uint8_t BitValue){
    GPIO_WriteBit(SCL_PORT, SDA_PIN, (BitAction)BitValue);
    Delay_ms(10);// 延迟10us 是为了可以让数据正确接收到
}

uint8_t customI2C_R_SDA(void){
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(SCL_PORT, SDA_PIN);
    Delay_ms(10);
    return BitValue;
}

// 开始 6 个时钟单元

/**
 * 1、 起始条件；
 *  确保SCL SDA 都确保释放
 *  先拉低SDA、在拉低SCL
 *  这样可以产生起始条件
 *
 */
void customI2C_Start(void){
    customI2C_W_SCL(1);
    customI2C_W_SDA(1);

    customI2C_W_SDA(0);
    customI2C_W_SCL(0);
}


/**
* 终止条件
 *  1、先拉低 SDA
 *  2、 释放 SCL
 *  3、释放 SDA
 *
*/
void customI2C_Stop(void){
    customI2C_W_SDA(0);

    customI2C_W_SCL(1);
    customI2C_W_SDA(1);
}

/**
 * 发送数据 是依次 把每一位放在 SDA 上,最高位在先
 *
 * 发送一个 字节时序开始时
 *  SCL 是低电平
 *  释放 SCL； 从机 会立刻将 SCL 读取
 *  然后再拉低 SCL 为 下移位放 数据做准备
 *
 * @param Byte
 */
void customI2C_SendByte(uint8_t Byte){
    for (int i = 0; i < 8; ++i) {
        // 第一次 & 0x80 ；第二次 0x40；  ... ;向 右 移动i 位 即可
        customI2C_W_SDA(Byte & (0x80 >> i));// 取出 Byte 最高位；所以 与 1000000也就是 0x80
        customI2C_W_SCL(1);// 释放
        customI2C_W_SCL(0);
    }
}


/**
 * 接收一个字节
 *  开始时 低电平； 此时从机需要把 数据放到 SDA 上； 主机要先 释放 SDA； 在 SCL 低电平 时， 从机会把数据放到 SDA 上
 *  1、主机释放SDA
 *  2、 SCL 高电平
 *  3、读取 SDA 上的数据，读取一位 后再把SCL 拉低；循环 读取 8 次
 *
 */
uint8_t customI2C_ReceiveByte(void){
    uint8_t Byte = 0x00;
    uint8_t i = 0x00;
    customI2C_W_SDA(1);// 主机释放SDA

    for (i = 0; i < 8; ++i) {
        customI2C_W_SCL(1);// SCL 高电平
        //读取 SDA 上的数据 如果等于 1
        if (customI2C_R_SDA() == 1){
            Byte |= (0x080 >> i);
        }
        //读取一位 后再把SCL 拉低
        customI2C_W_SCL(0);
    }
    return Byte;
}


/**
 * 发送应答； 发送1 位
 * 函数进来时 SCL 低电平 进入
 *  1、主机把 AckBit 放到 SDA 上
 *  2、释放 SCL
 *  3、 再拉低SCL
 *
 * @param AckBit
 */
void customI2C_SendACK(uint8_t AckBit){
    customI2C_W_SDA(AckBit);//
    customI2C_W_SCL(1);// 释放
    customI2C_W_SCL(0);
}

/**
 * 接收 应答
 * 函数进来时  SCL 低电平
 * 1、 主机 释放SDA 防止干扰从机；  ***** 主机 输出1 并不是 强制 SDA 位高电平； 而是释放 SDA ****
 * 2、SCL 高电平
 * 3、 主机开始读取数据
 * 4、SCL 电平拉低， 进入下一个时序
 *
 */

uint8_t customI2C_ReceiveAckByte(void){
    uint8_t AckBit;
    customI2C_W_SDA(1);// 主机 释放SDA 防止干扰从机；
    customI2C_W_SCL(1);// SCL 高电平
    //读取 SDA 上的数据 如果等于 1
    AckBit = customI2C_R_SDA();
    //读取一位 后再把SCL 拉低
    customI2C_W_SCL(0);
    return AckBit;
}
