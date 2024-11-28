#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "timer_r.h"
#include "FREERTOS_QUEUE.h"



int main(void) {
    OLED_Init();
    FREERTOS_QUEUE_MAIN();
    while(1) {


    }
}
