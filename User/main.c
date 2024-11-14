#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "ADC_R.h"

uint16_t ADValue;
float Voltage;
int main(void) {
    OLED_Init();
    ADC_R_INIT();
    OLED_ShowString(1, 1, "ADValue:");
    OLED_ShowString(2, 1, "Volatge:0.00V");
    while(1) {
        ADValue = ADC_R_GetValue();
        Voltage = (float)ADValue / 4095.8 * 3.3;
        OLED_ShowNum(1, 9, ADValue, 4);
        OLED_ShowNum(2, 9, Voltage, 1);
        OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
    }
}
