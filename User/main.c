#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "timer_r.h"
#include "FREERTOS_QUEUE_SET.h"
#include "FREERTOS_TEMP.h"

int main(void) {
    OLED_Init();
//    OLED_ShowString(1,1,"abc");
//    FREERTOS_QUEUE_SET_MAIN();
FREERTOS_TEMP_MAIN();
    while(1) {
    }
}
