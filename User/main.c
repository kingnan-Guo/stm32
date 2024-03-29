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
int main(void) {
    OLED_Init();
    AD_Init();

    OLED_ShowString(1, 1, "ADValue:");
    OLED_ShowString(2, 1, "Volatge:0.00V");
    while(1) {

        ADValue = AD_GetValue();
        OLED_ShowNum(1, 9, ADValue, 4);

    }
}


