#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "customizeInterrupt.h"

#include "xTaskCreateFun2.h"
int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
//    xTaskCreateFun2Main();

    Interrupt_Init();
    OLED_ShowNum(1, 1,  1, 5);
    while(1) {
        OLED_ShowNum(3, 1,  getNum(), 5);
    }
}





