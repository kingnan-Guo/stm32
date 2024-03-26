/**
 * 输入捕获
 *  使用 GPIO  PA6
 *  设置输入模式 上拉输入  或 浮空输入
 *
 *  配置 时基单元 让 CNT 在内部时钟的驱动下自增运行
 *
 *  选择 从模式 触发源 TI1FP1 执行 Rest
 *
 *
 *  最后调用 TIM_Cmd 开启定时器
 */
#include "stm32f10x.h"
/**
 * 1、开启时钟
 * 2、配置GPIO
 * 3、配置时基单元
 */
void IC_Init(void){

    // 开启时钟 TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /**
     * 配置GPIO
     * 因为 引脚 要使用 GPIO 所以 先初始化 GPIO
     * ITM3_CH1 对应 引脚 PA6
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM3);// 选择内部时钟 TIM3

    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period =  65536 - 1; // 周期 ARR 自动重装器的值 (有一个数的偏差 ，取值 要早65535 之间)； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72  - 1;// 720 分频 1MHZ ；PSC 预分频 的值 (有一个数的偏差 ，取值 要早65535 之间)；  对 72MHZ 进行7200 分频 得到10k 计数频率； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;// 重复计数器的值 （高级计数器才有的值）
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);// 时基单元 初始化

    // 配置 初始化 输入
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;// 选择 通道 TIM_Channel_1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;// 配置输入捕获的滤波器 0x0 ～ 0xF 值越大 滤波效果 越好
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//极性 ； 边沿检测 极性检查； 选择  TIM_ICPolarity_Rising 上升沿触发 还是 TIM_ICPolarity_Falling下降沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//分频器  不分频 ； 就是每 上升沿 都有效 TIM_ICPSC_DIV1
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;// 选择 触发信号 从 哪个 引脚 输入 ； 这个参数 是配置 输入选择器的 ； 可以选择 直连通道 或者 是 交叉通道
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /**
     * 主从模式 配置 好
     * 配置 TRGI 的触发源 为 TIF1FP1
     * 配置触发源 TIM_SelectInputTrigger
     *
     * 当前选择 TIM_TS_TI1FP1
     */
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

    /**
     * 配置 从模式 Reset
     * TIM_SlaveMode_Reset
     */
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);


    //启动定时器
    TIM_Cmd(TIM3, ENABLE);

}

// 计算频率
uint32_t IC_GetFreq(void){
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
