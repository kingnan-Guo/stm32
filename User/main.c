#include "stm32f10x.h"

#include "OLED.h"
#include "delay.h"
#include "Serial.h"
#include "retarget.h"

uint8_t  data = 0x20;
int16_t num = 0x11;
uint8_t Arr[] = {0x41, 0x42, 0x43};

uint8_t RxData;

int32_t age;
int main(void) {
    OLED_Init();
    Serial_Init();
    /**
     * 注册 USART1 用于重定向
     * 当然其他端口也行
     */
    RetargetInit(USART1);
    OLED_ShowString(1, 1, "receive Data");

    while(1) {
//        Serial_Print("Num=%d\r\n",  num++);
//        Serial_SendArr(Arr, 3);
//        Serial_SendString("\r\n");
//        // 传输字符串的时候会自动补上 结束标志位
//        Serial_SendString("StringData\r\n");
//        Serial_SendString((char *) &data);
//        Serial_SendString("\r\n");
//        Serial_SendNumber(200, 3);
//        Serial_SendString("\r\n");
//
//        printf("Hello World!\n");
//        printf("printfNum=%d\r\n",  data);
//
//        data++;
//        Serial_SendByte(data);
//        OLED_ShowHexNum(2, 1, data, 3);
//
//        Delay_ms(1000);

        // 循环判断标志位
        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
            // 读取数据 自动清除 标志位
            RxData = USART_ReceiveData(USART1);
             OLED_ShowHexNum(2,1, RxData, 5);
        }

//        printf("printfNum=%d\r\n",  data);
//        Delay_ms(1000);
    }
}

