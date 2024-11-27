#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "timer_r.h"
#include "FREERTOS_LIST.h"



int main(void) {
    OLED_Init();
    FREERTOS_LIST_INIT_MAIN();
    while(1) {


    }
}
