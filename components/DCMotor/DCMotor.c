/**
*  B6612 电机驱动模块
 *  引脚 ****
 *  VM 电机电源 ; 接在 stlink 5V
 *  VCC 逻辑电源 接在3.3V
 *  GND 电源负极 面包板 辅机
 *  AO1 直流电机
 *  AO2 直流电机
 *  STBY 待机控制引脚 ；不需要待机所以直接接 3.3v
 *  AIN1 (方向控制引脚) 任意两个GPIO ； 接A4
 *  AIN2 (方向控制引脚) 任意两个GPIO ； 接A5
 *  PWMA 速度控制 ； 需要接PWM 的输出脚； 接 PA2 ； PA2 对应 TIM2 的通道3; 增加 PWMA 频率 提高速度
*/
#include "stm32f10x.h"
#include "PWM.h"

void Motor_Init(void){



    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;// 引脚 4 5
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    PWM_Init();
}

void Motor_SetSpeed(float speed){
    if(speed > 0){
        GPIO_SetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare3(speed);
    }
    else{
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        PWM_SetCompare3(-speed);
    }
}

