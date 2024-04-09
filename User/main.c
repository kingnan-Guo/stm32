#include "stm32f10x.h"

#include "OLED.h"
#include "delay.h"
#include "Serial_package.h"
#include "retarget.h"
#include "delay.h"
#include "key.h"

//uint8_t  data = 0x20;
//int16_t num = 0x11;
//uint8_t Arr[] = {0x41, 0x42, 0x43};

uint8_t RxData;
uint8_t KeyNum;

int main(void) {
    OLED_Init();
    Serial_package_Init();
    Key_Init();
    RetargetInit(USART1);
    /**
     * 注册 USART1 用于重定向
     * 当然其他端口也行
     */
    OLED_ShowString(1, 1, "TxPacket ");
    OLED_ShowString(3, 1, "RxPacket ");
    // 发送缓存区数组
    Serial_package_TxPacket[0] = 0x01;
    Serial_package_TxPacket[1] = 0x02;
    Serial_package_TxPacket[2] = 0x03;
    Serial_package_TxPacket[3] = 0x04;
    Serial_package_SendPacket();
    while(1) {
        KeyNum = Key_GetNum();
        if(KeyNum == 1){
            Serial_package_TxPacket[0]++;
            Serial_package_TxPacket[1]++;
            Serial_package_TxPacket[2]++;
            Serial_package_TxPacket[3]++;

            Serial_package_SendPacket();

            OLED_ShowHexNum(2,1,Serial_package_TxPacket[0], 2);
            OLED_ShowHexNum(2,4,Serial_package_TxPacket[1], 2);
            OLED_ShowHexNum(2,7,Serial_package_TxPacket[2], 2);
            OLED_ShowHexNum(2,10,Serial_package_TxPacket[3], 2);


        }


        if(Serial_package_GetRxFlag() == 1){
            OLED_ShowHexNum(4,1,Serial_package_RxPacket[0], 2);
            OLED_ShowHexNum(4,4,Serial_package_RxPacket[1], 2);
            OLED_ShowHexNum(4,7,Serial_package_RxPacket[2], 2);
            OLED_ShowHexNum(4,10,Serial_package_RxPacket[3], 2);
        }
//        printf("end \n");
//        Delay_ms(1000);
    }
}


