#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "PWM_R.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
    OLED_ShowNum(1,1,2, 5);
    PWM_R_Init();
//    while(1) {
//        // OLED_ShowNum(1,1,getTIM2Count(), 5);
//        // OLED_ShowNum(2,1,getExtiInterruptCount(), 5);
//    }
}

