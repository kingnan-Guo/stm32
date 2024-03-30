#include "stm32f10x.h"

#include "OLED.h"
#include "AD.h"


uint16_t ADValue;
float Voltage;

uint16_t AD0, AD1, AD2;



int main(void) {
    OLED_Init();
    AD_Init();

    OLED_ShowString(1, 1, "AD0:");
    OLED_ShowString(2, 1, "AD1:");
    OLED_ShowString(3, 1, "AD2:");
    while(1) {

        AD0 = AD_GetValue(ADC_Channel_0);
        AD1 = AD_GetValue(ADC_Channel_1);
        AD2 = AD_GetValue(ADC_Channel_2);

        OLED_ShowNum(1, 5, AD0, 4);
        OLED_ShowNum(2, 5, AD1, 4);
        OLED_ShowNum(3, 5, AD2, 4);

    }
}


