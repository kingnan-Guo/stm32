#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
//#include "countSensor.h"
#include "Timer.h"
#include "servo.h"
#include "key.h"

uint8_t i;
uint8_t keyNum;
float Angel;

int main(void) {
//    OLED_Init();
//    servo_init(void)
//    OLED_ShowChar(1, 1, 'Y');
//    OLED_ShowChar(1, 2, 'e');
//    OLED_ShowString(1, 4, "miaomiao");
//    OLED_ShowNum(2, 1, birth,8);
//    OLED_ShowSignedNum(3, 1, -88, 2);
//    OLED_ShowHexNum(3, 5, 0xAABB, 4);     ·`
//    OLED_ShowBinNum(4, 1, 0xABAB, 16); // 十六 进制 转 二进制
//    Delay_ms(2000);
//    OLED_Clear();
//    LED_Init();
    Key_Init();
    OLED_Init();
    servo_init();

    OLED_ShowString(1, 4, "miaomiao");
//    Angel += 173;
	while(1) {
        keyNum = Key_GetNum();
//        if(keyNum == 1){
//            Angel += 30;
//            if (Angel >180){
//                Angel = 1000;
//            }
//        }
        servo_setAngle(Angel);
        OLED_ShowNum(2, 1, Angel,3);
    };
};
