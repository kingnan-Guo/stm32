/**
 * 1、开启时钟 UART和GPIO时钟打开 USART1 APB2 、GPIO APB1
 * 2、GPIO 初始化、TX配置复用输出、RX 配置输入
 * 3、配置USART 直接使用一个结构体
 * 4、如果只需要发送功能 直接开启USART ，初始化结束
 * 5、如果配置接收的功能，需要配置中断，在开启UART之前 添加 ITConfig 的NVIC的代码
 *
 * 6、初始化完成之后 如果要发送，就调发送函数、如果要接收 就调接收函数
 * 7、如果要获取发送和接收的状态，就调用获取  标志位的函数
 *
 *
 */
#include "stm32f10x.h"
#include <stdio.h>
#include "stdarg.h"
//#include <pthread.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;








void   Serial_Init(void){
    // USART1 是 APB2 的外设
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 开启GPIO 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /**
     * GPIO 初始化
     *
     * RX输入：
     * Rx选择输入模式； 一根线只能有一个输出但可以有多个输入，
     * 所以输入脚GPIO；外设可以同时用
     * 一般RX配置为浮空输入或者上拉输入， 因为 串口模式 空闲状态情况下是高电平，所以不能使用下拉输入，
     *
     *  TX输出：
     *      复用推挽输出，供USART的TX使用
     *
     * 当前先用输出，所以只初始化 PA9
     */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /**
     * 初始化 A10  用于接收数据
     */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    //初始化USART
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;// 波特率 USART_Init 会自动算好对应 9600 的分频，然后写到 BRR 寄存器
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控 不使用 流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//串口模式 如果需要发送 又需要接收，那么 = USART_Mode_Rx ｜ USART_Mode_Tx
    USART_InitStructure.USART_Parity = USART_Parity_No;// 校验位 不使用USART_Parity_No
    USART_InitStructure.USART_StopBits = USART_StopBits_1;// 停止位  USART_StopBits_1 一位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;// 字长 位数 8 位
    USART_Init(USART1, &USART_InitStructure);


    /**
     * 1、当 USART_IT_RXNE 标志位 置 1 就会向 NVIC 申请中断
     * 2、在中断函数中 接收数据，具体中断函数的名字要在 启动函数中查找
     *
     */
    // 配置串口中断  ; USART_IT_RXNE: Receive Data register not empty interrupt 接收数据寄存器非空中断， 开启USART_IT_RXNE 到中断标志位的输出;
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    // 配置NViV
    // NVIC_PriorityGroup_2 两位抢占 两位响应
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 初始化 NVIC的 USART1 通道
    // 结构体配置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//指定 中断 通道开启 或 关闭 ； USART1_IRQn 在 Libraries/CMSIS/stm32f10x.h 中 查询； 指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//指定 中断 通道 是 使能 还是  失能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;// 指定 所选通道的抢占 优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// 指定 所选通道的  响应优先级
    NVIC_Init(&NVIC_InitStructure);

    // 串口使能
    USART_Cmd(USART1,ENABLE);

}


/**
 * 发送一个字节的数据
 *
 *
 *
 * =========
 * TXE:发送数据寄存器空 (Transmit data register empty)
 * 当TDR寄存器中的数据被硬件转移到移位寄存器的时候，该位被硬件置位。
 *
 * 如果USART_CR1,寄存器中的TXEIE为1，则产生中断。
 *
 * 。对USART_DR的写操作，将该位清零。
 *      0：数据还没有被转移到移位寄存器；
 *      1：数据已经被转移到移位寄存器。
 *      注意：单缓冲器传输中使用该位。
 *
 * @param Byte
 */
void Serial_SendByte(uint8_t Byte){
    // byte 数据写如带 TDR
    USART_SendData(USART1, Byte);
    // 发送之后我们需要等待一下 标志位 : USART_FLAG_TXE 发送移位寄存器标志位 空
    // 标志位 置 1 之后不需要手动清零，在下一次 USART_SendData 会自动清零
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

// 发送数组
void Serial_SendArr(uint8_t *Array, uint16_t Length){
    // uint16_t i;
    for (int j = 0; j < Length; ++j) {
        Serial_SendByte(Array[j]);
    }
}
//发送字符串
void Serial_SendString(char *String){
    // 多硬空字符 ，是字符串结束标志位
    // 也可以： i < String[i] != '\0';
    for (int i = 0; i < String[i] != 0; i++) {
        Serial_SendByte(String[i]);
    }
}


// 返回 X^Y
uint32_t Serial_Pow(uint32_t X, uint32_t Y){
    uint32_t  Result = 1;
    while (Y--){
        Result = Result * X;
    }
    return  Result;
}

// 发送数字
void Serial_SendNumber(uint32_t Number, uint8_t Length){
    // 将NUmber 以 个位、 十位、 百位 以十进制拆分开， 然后再换成字符数据，依次发送出去
    // 方法：取某一位就是 ： 数字 / (10^2) % 10
    uint8_t i;
    for (i = 0; i < Length; ++i) {
        // 还要偏移 0x30
        Serial_SendByte(Number/Serial_Pow(10, Length -i -1) % 10 + '0') ;
    }
}



/**
 * 可变参数 把printf 变成 可变参数
 * @param format 用来接收 格式化字符串
 * @param ... 用来接收后面的额可变参数列表
 *
 * 1、定义输出的字符串
 * 2、
 */
void Serial_Print(char *format, ...){
    // 定义输出的字符串
    char String[100];
    //定义一个参数列表变量 va_list 类型名 arg变量名
    va_list arg;
    // 从 format 位置开始接收参数表， 房子啊arg 里面
    va_start(arg, format);
    // vsprintf 打印位置是 String 格式化字符串 format； 参数 arg
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

//中断 函数  固定的名称
void USART1_IRQHandler(void){
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
        // 读取数据 自动清除 标志位
        Serial_RxData = USART_ReceiveData(USART1);
        Serial_RxFlag = 1;
        // 清除  标志位 ；如果 读取的DR 那么就会自动清除，但是这里再次清除一下
        USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);

    }
}

uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

uint8_t Serial_GetRxFlag(void){
    if(Serial_RxFlag == 1){
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}



