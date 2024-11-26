#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "xTaskSuspendResume.h"
#include "FREERTOS_LIST.h"
#include "FREERTOS_TEMP.h"
#include "timer_r.h"

int main(void) {
    OLED_Init();
//    xTaskSuspendResume_MAIN();
//    FreeRTOS_NVIC_MAIN();
//    FREERTOS_LIST_INIT_MAIN();

    FREERTOS_TEMP_INIT_MAIN();
    while(1) {
    }
}
