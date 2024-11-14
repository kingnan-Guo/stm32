#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "ENCODE_R.h"
#include "timer_r.h"


int main(void) {
    OLED_Init();
    Timer_R_Init();
    ENCODE_R_INIT();

//    OLED_ShowNum(1,1, 0, 5);
    while(1) {
        OLED_ShowSignedNum(2,5, TIM_GetCounter(TIM3), 5);
        OLED_ShowSignedNum(3,5, ENCODE_R_GET_Speed(), 5);

        OLED_ShowNum(4, 1, TIM_GetCounter(TIM2), 5);
    }
}
