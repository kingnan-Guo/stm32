#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "xTaskSuspendResume.h"
#include "FreeRTOS_NVIC.h"
#include "timer_r.h"

int main(void) {
    OLED_Init();
//    xTaskSuspendResume_MAIN();
    FreeRTOS_NVIC_MAIN();

    while(1) {
         Timer_R_MAIN_WHILE();
    }
}
