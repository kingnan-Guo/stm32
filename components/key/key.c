#include "stm32f10x.h"
#include "delay.h"

void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    // 接收 引脚 电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

uint8_t Key_GetNum(void){
    uint8_t keyNum = 0;
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0){
        Delay_ms(20);// 防抖
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);// 直到手放开
        Delay_ms(20);// 防抖
        keyNum = 1;
    }


    return keyNum;
}

