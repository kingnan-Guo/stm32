//
// Created by 90175 on 2024/11/12.
//
#include "stm32f10x.h"
#include "PWM_R.h"

void PWM_R_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化  引脚 推挽输出
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    // 开启 计数器


    TIM_InternalClockConfig(TIM2);


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // 由于要对 信号 进行采样 判定， 所以 使用 内部时钟的频率 作为采样的 频率， 而 TIM_ClockDivision 就是 将采样频率分频 划分，采样次数越多 精度越高，但是 延时 增加
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;// 指定时钟划分频  TIM_CKD_DIV1 一分频，
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 计数 模式， 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 100 -1 ;// ARR 计数器 重装 器的值； 要在  0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;// PSC 预分频器 的值; 这里目的 是 定时 一秒钟；  要在 0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;// 指定重复计数器值； 高级计数器 才有的 给 0
    //配置时机 单元
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);


    // 初始化  输出比较单元

//    TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);// 初始化默认参数
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;// 选择 PWM1 模式
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;// 输出比较极性 ; OC1 ref
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;// 输出使能 开启
    TIM_OCInitStructure.TIM_Pulse = 100;// CCR
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    //启动定时器
    TIM_Cmd(TIM2, ENABLE);

}

//  改变 通道 1 的占空比
void PWM_R_SetCompare1(uint16_t Compare){
    TIM_SetCompare1(TIM2, Compare);
}

//单独写 预分频 的 值 ; 改变频率
void PWM_R_SetPrescaler(uint16_t Prescaler){
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}




//#include "PWM_R.h"
//int main(void) {
//    // 初始化 I2C 的引脚
//    OLED_Init();
//    OLED_ShowNum(1,1,2, 5);
//    PWM_R_Init();
//    int i;
//    while(1) {
//        // OLED_ShowNum(1,1,getTIM2Count(), 5);
//        // OLED_ShowNum(2,1,getExtiInterruptCount(), 5);
//        for ( i = 0; i < 100; ++i) {
//            PWM_R_SetCompare1(i);
//            Delay_ms(10);
//        }
//        for (i = 0; i <= 100; i++)
//        {
//            PWM_R_SetCompare1(100 - i);
//            Delay_ms(10);
//        }
//    }
//}

