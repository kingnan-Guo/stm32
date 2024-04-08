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
//    RetargetInit(USART1);
    OLED_ShowString(1, 1, "receive Data");

    while(1) {

        // 循环判断标志位
        if (Serial_GetRxFlag() == 1){
            // 读取数据 自动清除 标志位
            RxData = Serial_GetRxData();
            OLED_ShowHexNum(2,1, RxData, 5);
        }

        //printf("printfNum=%d\r\n",  data);
        //Delay_ms(1000);
    }
}

