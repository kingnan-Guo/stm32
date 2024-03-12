#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
//#include "countSensor.h"
#include "Timer.h"
#include "PWM.h"

uint8_t i;

int main(void) {
//    OLED_Init();
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
    pwm_Init();
	while(1) {
        for (int i = 0; i <= 100; ++i) {
            PWM_SetComplare1(i);
            Delay_ms(100);
        };
        for (int i = 0; i <= 100; ++i) {
            PWM_SetComplare1(100 - i);
            Delay_ms(100);
        };
    };
};
