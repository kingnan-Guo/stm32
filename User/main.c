#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "PWM_R.h"


int main(void) {
    OLED_Init();
    PWM_R_Init();
    int i;
    while(1) {
        for ( i = 0; i < 100; ++i) {
            PWM_R_SetCompare1(i);
            OLED_ShowNum(1,1,i, 5);
            Delay_ms(10);
        }
        for (i = 0; i <= 100; i++)
        {
            PWM_R_SetCompare1(100 - i);
            OLED_ShowNum(1,1,100 - i, 5);
            Delay_ms(10);
        }
    }
}

