//
// Created by 90175 on 2024/11/7.
//

#include "LER_R.h"
#include "stm32f10x.h"
#include <iostream>


void LED_R_Init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void LED_R_RESET(){
    GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
}

void LED_R_SET(){
    GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
}

void LED_R_test(){
//    new LED_R().run();
    LED_R().run();
}


void LED_R::run() {
    LED_R_RESET();
}

