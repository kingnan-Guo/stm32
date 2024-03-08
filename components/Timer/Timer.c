/**
 * 定时器使能  后 计数器 开始计数
 * 当 计数器更新时  触发中断
 *
 *
 *
 */
 /**
  *  初始化流程
  *  1、RCC 开启时钟 打开时钟后 定时器的基本时钟 和 整个 外设的工作时钟 就会同时打开
  *  2、选择时基单元 的时钟源 对于定时中断 我们就选择 内部时钟源
  *  3、配置时基单元 包括 预分频器 自动重装器 计数模式 用一个结构体 就可以配置
  *  4、 配置输出中断 控制 允许更细中断输出到nvic
  *  5、配置 nvic  在nvic中打开定时器中断通道 并分配一个优先级
  *  6、 运行控制
  *  6、 运行控制
  *
  *
  */
#include "stm32f10x.h"


void Timer_Init(void){
    // 开启时钟
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM2);// 选择内部时钟 TIM2
    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period = ;
    TIM_TimeBaseInitStructure.TIM_Prescaler = ;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = ;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure)

}

//定时器中断 函数


