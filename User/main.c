#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
#include "Timer.h"
#include "key.h"
#include "PWM.h"
#include "IC.h"

int main(void) {


    OLED_Init();
    PWM_Init();
    IC_Init();

    PWM_SetPrescaler(720 -1); // 频率 Freq = 72MHZ / (psc + 1) / (Arr + 1) ; 当前 (Arr + 1) = 100 ; == 1KHZ
    PWM_SetCompare1(50);  // 占空比  Duty =  RCC / （ARR + 1）   == 50

    OLED_ShowString(1, 1, "FReq: 00000 HZ");
    while(1) {
        OLED_ShowNum(1, 7, IC_GetFreq(),5);
    }
}
