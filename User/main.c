#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>


int main(void) {
    OLED_Init();
    while(1) {
        OLED_ShowNum(1, 1, 0, 9);
    }
}
