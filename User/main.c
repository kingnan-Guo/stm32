#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "xTaskCreateStaticFun.h"

int main(void) {
    OLED_Init();
    xTaskCreateStaticFun_Main();

    while(1) {
    }
}
