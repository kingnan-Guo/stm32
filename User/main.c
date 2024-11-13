#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "PWM_R.h"
#include "INPUTCAPTURE_R.h"


int main(void) {
    OLED_Init();

    PWM_R_Init();
    INPUTCAPTURE_R_INIT();



    OLED_ShowNum(1,1,0, 5);
    PWM_R_SetPrescaler(720 -1); // 频率 Freq = 72M / (PSC + 1) / 100
    PWM_R_SetCompare1(50); // 占空比 Duty = 50;
    while(1) {
    }
}

