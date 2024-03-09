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
  * 计数器溢出频率  CK_CNT_OV = CK_CNT/(ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
  */
#include "stm32f10x.h"
//extern  uint16_t number_timer_interrupt;// extern 告诉编译器 已经 声明过 number_timer_interrupt 只不过是在其他的文件中

void Timer_Init(void){
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM2);// 选择内部时钟 TIM2
    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period =  10000 - 1; // 周期 ARR 自动重装器的值 (有一个数的偏差 ，取值 要早65535 之间)； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200  - 1;// PSC 预分频 的值 (有一个数的偏差 ，取值 要早65535 之间)；  对 72MHZ 进行7200 分频 得到10k 计数频率； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;// 重复计数器的值 （高级计数器才有的值）
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // 手动清除更新中断 标志位 ，就能避免 刚初始化完成就进入中断 的问题
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // 使能 更新中断   TIM2 选择时钟  ； TIM_IT_Update 更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // nvic  打通中断通道
    // 1、nvic 优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 2、
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;// 定时器 2  在nvic里的通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// 响应 优先级
    NVIC_Init(&NVIC_InitStructure);

    //启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

////定时器中断 函数
////在启动 文件里找 TIM2_IRQHandler ; 当定时器产生更新中断时 那么就会执行这个函数
//void TIM2_IRQHandler(void){
//    // 检测中断标志位 ; TIM_GetITStatus 获取中断标志位 ； TIM2 选择的时钟； TIM_IT_Update 哪种 中断方式
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
//        number_timer_interrupt ++;
//        // 清除 中断 标志位
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//
//}
