//
// Created by 90175 on 2024/11/11.
//
#include "stm32f10x.h"
#include "timer_r.h"
#include "OLED.h"
#include "delay.h"
uint16_t time2InterruptCount = 0;
void Timer_R_Init(){
    // 使用 Tim2 定时器
    // RCC_APB1Periph_TIM2 RCC的 APB1 的 Periph(外设)   TIM2(定时器2 外设)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 刚才 这里 写的 有错误， 写成了 RCC_APB1PeriphResetCmd；这个 是用来 复位 外设的

    // 选择时机单元的时钟 :  内部时钟
    // 定时器 上电 后 默认 就是内部时钟 ，所以 可以不写
    TIM_InternalClockConfig(TIM2); // TIM2 的时机单元由  内部时钟 来驱动


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // 由于要对 信号 进行采样 判定， 所以 使用 内部时钟的频率 作为采样的 频率， 而 TIM_ClockDivision 就是 将采样频率分频 划分，采样次数越多 精度越高，但是 延时 增加
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;// 指定时钟划分频  TIM_CKD_DIV1 一分频，
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 计数 模式， 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 10000 -1 ;// 计数器 重装 器的值； 要在  0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;// 预分频器 的值; 这里目的 是 定时 一秒钟；  要在 0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;// 指定重复计数器值； 高级计数器 才有的 给 0
    //配置时机 单元
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

    // 手动清除更新中断 标志位 ，就能避免 刚初始化完成就进入中断 的问题
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // TIM_ITConfig   使能 中断 计数器 , TIM_IT选择 配置哪个中断输出; 这里 就开启可 更新中断到 NVIC 的 通道
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);// TIM_IT_Update 更新中断

    // 接下来 配置  NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;// 指定通道; 在stm32f10x.h文件中，找到 对应的通道，定时器 2 在 NVIC 里的通道 TIM2_IRQn
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;// 指定中断通道 使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;// 抢占 优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;// 响应 优先级
    NVIC_Init(&NVIC_InitStruct);


    // 启动 定时器
    TIM_Cmd(TIM2, ENABLE);

}


uint16_t getTime2InterruptCount(){
    return time2InterruptCount;
}

// 跟  restarComponets/EXTI_Interrupt/EXTI_Interrupt.c 和 components/Timer/Timer.c 的 中断函数 重复了 所以 ，这里注掉
void TIM2_IRQHandler(void){
    /// TIM_IT_Update 代表 要看 哪个中断标志位
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        time2InterruptCount++;
        // 清除标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


// ===============

void Timer_R_MAIN(void) {
    // 初始化 I2C 的引脚
    Timer_R_Init();
//    while(1) {
//        OLED_ShowNum(1,1,getTime2InterruptCount(), 5);
//        OLED_ShowNum(4,1,TIM_GetCounter(TIM2), 5);
//    }
}

void Timer_R_MAIN_WHILE(void) {
    OLED_ShowNum(3,1,getTime2InterruptCount(), 5);
    OLED_ShowNum(4,1,TIM_GetCounter(TIM2), 5);
}
