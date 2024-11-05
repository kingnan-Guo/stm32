#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"

#include "xTaskCreateFun2.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
//    OLED_ShowString(1, 1, "cubeMX_original");

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);




//    xTaskCreateFun2Main();
    while(1) {
        GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_RESET);
        Delay_s(1);
        GPIO_WriteBit(GPIOC, GPIO_Pin_14, Bit_SET);
        Delay_s(1);
    }
}





