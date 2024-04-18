#include "stm32f10x.h"

#include "OLED.h"
//#include "delay.h"


int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
    OLED_ShowString(1, 1, "start ");
    while(1) {

    }
}



/**
 *
 *
 *
 */

