#include "stm32f10x.h"

void LED_Init(void){
    // 创建时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13);

}

void LED1_ON(void){
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void LED1_OFF(void){
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void LED1_Turn(void){
    // 查看输出 pin 的电平
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0){
        LED1_OFF();
    } else {
        LED1_ON();
    }
}

