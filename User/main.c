#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
//#include "countSensor.h"
#include "Timer.h"
#include "servo.h"

uint8_t i;

int main(void) {
//    OLED_Init();
//    servo_init(void)
//    OLED_ShowChar(1, 1, 'Y');
//    OLED_ShowChar(1, 2, 'e');
//    OLED_ShowString(1, 4, "miaomiao");
//    OLED_ShowNum(2, 1, birth,8);
//    OLED_ShowSignedNum(3, 1, -88, 2);
//    OLED_ShowHexNum(3, 5, 0xAABB, 4);
//    OLED_ShowBinNum(4, 1, 0xABAB, 16); // 十六 进制 转 二进制
//    Delay_ms(2000);
//    OLED_Clear();
//    LED_Init();

    OLED_Init();
    servo_init();
    servo_setAngle(90);
    OLED_ShowString(1, 4, "miaomiao");

	while(1) {
        keyNUm = key_GetName();
        if(kekeyNUm == 1){
            Angel += 30;
            if (Angel >180){

                Ange = 1000;

            }
        }
        servo_setAngle(90);
        OLED_ShowNum(1, 7, weke,3);
    };
};
