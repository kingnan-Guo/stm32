#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "FreeRTOSDemo.h"

int main(void) {
    OLED_Init();
    FreeRTOSDemoMain();

    while(1) {
    }
}
