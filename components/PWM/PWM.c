/**
 * PWM 频率 ： Freq = CK_PSC / (PSC + 1) / (ARR + 1);  CK_PSC = 72MHZ ;PSC  预分频; ARR 周期 ARR 自动重装器的值
 * PWM 占空比: Duty = CCR / (ARR + 1)
 * PWM 分辨率: Reso = 1 / (ARR + 1)
 *
 *
 * 在运行过程中 不断 改变CCR 那么 可以实现 呼吸灯 的效果
 */
#include "stm32f10x.h"

void pwm_Init(void){
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM2);// 选择内部时钟 TIM2

    /**
     * 重定向 相关
     * 1、首先开启 AFIO; AFIO:复用功能引脚重映射，外部中断引脚选择
     * 2、部分重映射1的方式
     * 3、 关闭 GPIOA_Pin_15 的 默认 功能 变为 普通引脚
     *
     * 4、 GPIO_Pin_0 要改成 GPIO_Pin_15 才能使用； GPIO_InitStructre.GPIO_Pin = GPIO_Pin_0; => GPIO_InitStructre.GPIO_Pin = GPIO_Pin_15;
     */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    // 有四种重定向的方式
//    // 这里选择 部分重映射1的方式
//    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//
//    //因为 GPIOA_Pin_15 有 默认复用 为 调试 JTDI ，所以若是 想要 GPIOA_Pin_15 为普通的 GPIO 那么也要做 重映射
//    // 使用 GPIO_PinRemapConfig 配置 GPIO_Remap_SWJ_JTAGDisable ；关闭JTAG ；保留 SW ；因为sw 还要作为 st-link 烧录程序
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);



    /**
     * 因为 引脚 要使用 GPIO 所以 先初始化 GPIO
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructre;
    GPIO_InitStructre.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructre.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推挽输出，要用定时器直接控制引脚； 输出控制权转移给片上外设；PWM的波形才能通过引脚输出
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    /** 选择ETR通过外部的时钟模式 2 输入的时钟;
     * 第二个参数 TIM_ExtTRGPSC_OFF 不需要分频；还有其他的参数可以选择
     * 第三个参数 外部触发的极性 参数有 反向 和不反向  也就是 TIM_ExtTRGPolarity_Inverted 低电平或下降沿有效 /  TIM_ExtTRGPolarity_NonInverted 高电平 或上升沿 有效
     * 第四个参数 滤波器  采样电判定 高低电平 ； 以一个采样频率 F ，采样 N 个点 ； 如果N 个点 都一样 才会有效输出 ;这个值  就是 来决定  F 和 N 的
     *   0x00 是  不用滤波器
    */
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);

    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;// 计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period =  100 - 1; // 周期 ARR 自动重装器的值 (有一个数的偏差 ，取值 要早65535 之间)； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720  - 1;// PSC 预分频 的值 (有一个数的偏差 ，取值 要早65535 之间)；  对 72MHZ 进行7200 分频 得到10k 计数频率； 在 10k 的频率下 计数10000 就是 1s
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
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    //启动定时器
    TIM_Cmd(TIM2, ENABLE);
}


void PWM_SetComplare1(uint16_t Compare){
    TIM_SetCompare1(TIM2,  Compare);
};
