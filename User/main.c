#include "stm32f10x.h"
//#include "LED.h"
#include "delay.h"
#include "OLED.h"
#include "Timer.h"
//#include "key.h"
//#include "PWM.h"
//#include "IC.h"
#include "Encode.h"
#include "AD.h"


uint16_t ADValue;
float Voltage;

int main(void) {
    OLED_Init();
    AD_Init();

    OLED_ShowString(1, 1, "ADValue:");
    OLED_ShowString(2, 1, "Volatge:0.00V");
    while(1) {

        ADValue = AD_GetValue();
        Voltage = (float)ADValue / 4095.8 * 3.3; // 4096 对应 AD 的范围值 ；但是比4096 小一点 所以时4095.5
        OLED_ShowNum(1, 9, ADValue, 4);

//        OLED_ShowNum(2, 9, (uint16_t)(Voltage * 1000) % 1000, 3);
        OLED_ShowNum(2, 9, Voltage, 1);
        OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
//        Delay_ms(100);
    }
}


