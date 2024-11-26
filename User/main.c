#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "xTaskSuspendResume.h"
//#include "FREERTOS_LIST.h"
#include "FreeRTOS_NVIC.h"
#include "timer_r.h"
#include "FREERTOS_LIST.h"


int main(void) {
    OLED_Init();

    FREERTOS_NVIC_MAIN();
    while(1) {

    }
}
