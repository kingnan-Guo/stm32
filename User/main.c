#include "stm32f10x.h"

#include "OLED.h"
#include "Timer.h"
#include "Serial.h"


int main(void) {
    OLED_Init();
    Serial_Init();



    Serial_SendByte(0x42);
    OLED_ShowString(1, 1, "0x42");
    while(1) {


    }
}


