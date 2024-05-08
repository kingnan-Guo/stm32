#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOSDemo.h"
#include "xTaskCreateFun.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
    OLED_ShowString(1, 1, "cubeMX_original");

    xTaskCreateFunMain();

//    FreeRTOSDemoMain();
    while(1) {

    }
}





