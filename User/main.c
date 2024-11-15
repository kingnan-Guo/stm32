#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "DMA_R.h"

int main(void) {
    OLED_Init();
    SHOW_DATA_INIT();
    while(1) {
        SHOW_DATA_WHILE();
    }
}


