//
// Created by 90175 on 2024/11/15.
//
//初始化 GPIO 开启 TXD RXD
//初始化串口




#include "stm32f10x.h"
#include "USART_R.h"

uint8_t USART_R_RXDATA;
uint8_t USART_R_RXFLAG; // 自定义标志位

void USART_R_INIT(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;// 复用推挽输出， 带上拉或者下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 初始化  UART
    USART_InitTypeDef USART_InitStruct;
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 9600;// 波特率
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制模式 ，不启用， 有两个控制引脚 控制 是否接收完成，是否可以发送
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;// 模式 发送 和 接收
    USART_InitStruct.USART_Parity = USART_Parity_No;// 指定奇偶校验模式。
    USART_InitStruct.USART_StopBits = USART_StopBits_1;// 停止位 是 1 为位
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;// 长度
    USART_Init(USART1, &USART_InitStruct);



    // 配置 中断; E
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;// 指定 中断 通道
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);


    USART_Cmd(USART1, ENABLE);
}

// 发送字节
void USART_R_SEND_BYTE(uint8_t byte){
    USART_SendData(USART1, byte);
    // 判断 TXE 中断标志位， 在 下一次 USART_SendData 会自动清除
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE));
}

void USART_R_SEND_ARRAY(uint8_t * Array, uint16_t Length){
    for (int i = 0; i < Length; ++i) {
        USART_R_SEND_BYTE(Array[i]);
    }
}

void USART_R_SEND_STRING(char *string){
    for (int i = 0; i < string[i] != 0; ++i) {
        USART_R_SEND_BYTE(string[i]);
    }
}

// 返回 X^Y
uint32_t _Pow(uint32_t X, uint32_t Y){
    uint32_t  Result = 1;
    while (Y--){
        Result = Result * X;
    }
    return  Result;
}

void USART_R_SEND_NUMBER(uint32_t Number, uint8_t Length){
    uint8_t i;
    for (int j = 0; j < Length; ++j) {
        USART_R_SEND_BYTE(
            Number/_Pow(10, Length -i -1) % 10 + '0'
        );
    }
}

uint8_t GET_RECEIVE_DATA(){
    return USART_R_RXDATA;
}

uint8_t GET_USART_R_RXFLAG(){
    if(USART_R_RXFLAG == 1){
        USART_R_RXFLAG = 0;
        return 1;
    }
    return 0;
}


// 运行时  _____USART1_IRQHandler 改成 USART1_IRQHandler
void _____USART1_IRQHandler(void){
    if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET){
        USART_R_RXDATA = USART_ReceiveData(USART1);
        USART_R_RXFLAG = 1;
        // 清除 标志位
        USART_ClearFlag(USART1, USART_FLAG_TXE);
    }
}


