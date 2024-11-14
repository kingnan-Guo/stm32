#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "ADC_MULTI_CHANNEL.h"

uint16_t ADValue;
float Voltage;
int main(void) {
    OLED_Init();
    ADC_MULTI_CHANNEL_INIT();
    while(1) {
        OLED_ShowNum(1, 1, ADC_MULTI_CHANNEL_GetValue(ADC_Channel_0), 4);
        OLED_ShowNum(2, 1, ADC_MULTI_CHANNEL_GetValue(ADC_Channel_1), 4);
    }
}
