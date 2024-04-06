#include "stm32f10x.h"

#include "OLED.h"
#include "delay.h"
#include "Serial.h"
//#include "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/stdio.h"
//#include <stdarg.h>

uint8_t  data = 0x20;
uint8_t Arr[] = {0x41, 0x42, 0x43};
//uint8_t StringData = 'USART';


int32_t age;
int main(void) {
    OLED_Init();
    Serial_Init();


    OLED_ShowString(1, 1, "0x42");


    while(1) {
        Serial_SendArr(Arr, 3);
        Serial_SendString("\r\n");
        // 传输字符串的时候会自动补上 结束标志位
        Serial_SendString("StringData\r\n");
        Serial_SendString((char *) &data);
        Serial_SendString("\r\n");
        Serial_SendNumber(200, 3);
        Serial_SendString("\r\n");
        // printf("Num=%d\r\n",  data);
        Serial_Print("Num=%d\r\n",  620);
        data++;
        Serial_SendByte(data);
        OLED_ShowHexNum(2, 1, data, 3);

        Delay_ms(1000);

    }
}


