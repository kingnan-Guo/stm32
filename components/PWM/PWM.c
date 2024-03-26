/**
 * PWM 频率 ： Freq = CK_PSC / (PSC + 1) / (ARR + 1);  CK_PSC = 72MHZ ;PSC  预分频; ARR 周期 ARR 自动重装器的值
 * PWM 占空比: Duty = CCR / (ARR + 1)
 * PWM 分辨率: Reso = 1 / (ARR + 1)
 *
 *
 * 在运行过程中 不断 改变CCR 那么 可以实现 呼吸灯 的效果
 */
#include "stm32f10x.h"

void PWM_Init(void){
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);





    /**
     * 因为 引脚 要使用 GPIO 所以 先初始化 GPIO
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出，要用定时器直接控制引脚； 输出控制权转移给片上外设；PWM的波形才能通过引脚输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM2);// 选择内部时钟 TIM2


    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period =  100 - 1; // 周期 ARR 自动重装器的值 (有一个数的偏差 ，取值 要早65535 之间)； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720  - 1;// 36 分频 20KHZ ；PSC 预分频 的值 (有一个数的偏差 ，取值 要早65535 之间)；  对 72MHZ 进行7200 分频 得到10k 计数频率； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;// 重复计数器的值 （高级计数器才有的值）
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);


    /**
     * 初始化 输出比较单元
     *
     * 初始化 TIM_OCInitStructure 通过TIM_OCStructInit 函数赋初始值
     */
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);// 给 TIM_OCInitStructure 赋初始值
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置  输出比较的模式
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_High;//输出比较 极性;选择 REF 有效时 输出 高电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出 使能 ； 开启
    TIM_OCInitStructure.TIM_Pulse = 0;// 设置RCC寄存器的值 ； （Capture Compare Register）输出比较寄存器 ； 直译 脉冲；改变CCR来改变 占空比
    // 通道 2 初始化
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    // 可使用 此进行通道一 初始化，那么 这样就可以 同时两个通道 来输出 两个 pwm 了
    //TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //通道3  通道4 也是可以同时 使用的

    /**
     * 对于 统一个定时器 不同通道输出的 pwm
     * 因为 不同 通道共用 一个 计数器的， 所以 频率必须是 一样的
     *
     */

    //启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * 改变 通道 1 的占空比
 *
 * 设置 TIM_Pulse
 *
 * @param Compare
 */
void PWM_SetCompare1(uint16_t Compare){
    TIM_SetCompare1(TIM2,  Compare);
};

/**
 * 单独写 预分频 的 值 ; 改变频率
 * TIM_PSCReloadMode写入的模式 ： TIM_PSCReloadMode_Update 更新时 生效
 * @param Prescaler
 */
void PWM_SetPrescaler(uint16_t Prescaler){
    //
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}