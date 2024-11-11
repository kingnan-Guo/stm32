//
// Created by 90175 on 2024/11/11.
//

#include "timer_r.h"
#include "stm32f10x.h"

void Timer_Init(){
    // 使用 Tim2 定时器
    // RCC_APB1Periph_TIM2 RCC的 APB1 的 Periph(外设)   TIM2(定时器2 外设)
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择时机单元的时钟 :  内部时钟
    // 定时器 上电 后 默认 就是内部时钟 ，所以 可以不写
    TIM_InternalClockConfig(TIM2); // TIM2 的时机单元由  内部时钟 来驱动


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // 由于要对 信号 进行采样 判定， 所以 使用 内部时钟的频率 作为采样的 频率， 而 TIM_ClockDivision 就是 将采样频率分频 划分，采样次数越多 精度越高，但是 延时 增加
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;// 指定时钟划分频  TIM_CKD_DIV1 一分频，
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 计数 模式， 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 10000 -1 ;// 计数器 重装 器的值； 要在  0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;// 预分频器 的值; 这里目的 是 定时 一秒钟；  要在 0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;// 指定重复计数器值； 高级计数器 才有的 给 0
    //配置时机 单元
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // TIM_ITConfig   使能 中断 计数器  TIM_IT选择 配置哪个中断输出
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}
