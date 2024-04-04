#include "stm32f10x.h"

#include "OLED.h"
#include "delay.h"
#include "Serial.h"

uint8_t  data = 0x20;

int main(void) {
    OLED_Init();
    Serial_Init();


    OLED_ShowString(1, 1, "0x42");


    while(1) {
        data++;
        Serial_SendByte(data);
        OLED_ShowHexNum(2, 1, data, 3);

        Delay_ms(1000);

    }
}


