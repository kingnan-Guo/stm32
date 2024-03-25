#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
//#include "countSensor.h"
#include "Timer.h"
#include "servo.h"
#include "key.h"
#include "DCMotor.h"

uint8_t i;
//uint8_t keyNum;
float Speed;

int main(void) {

    Key_Init();
    OLED_Init();
    Motor_Init();

    Speed += 50;
    Motor_SetSpeed(Speed);


	while(1) {
        //keyNum = Key_GetNum();
        OLED_ShowNum(2, 1, Speed,3);
    };
};
