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
    PWM_R_SetPrescaler(720 -1); // 频率 Freq = 72M / (PSC + 1) / 100
    PWM_R_SetCompare1(42); // 占空比 Duty = 50;
    OLED_ShowNum(1,1, 0, 5);
    OLED_ShowNum(1,1, TIM_GetCapture1(TIM2), 5);
    OLED_ShowNum(2,1, TIM_GetPrescaler(TIM2), 5);
    while(1) {

        OLED_ShowNum(3,1,getICFreq(), 10);
        OLED_ShowNum(4,1,getICDuty(), 10);
    }
}
