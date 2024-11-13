//
// Created by 90175 on 2024/11/13.
// 输入捕获


#include "stm32f10x.h"
#include "INPUTCAPTURE_R.h"

// 初始化 步骤
//1、RCC 开启 GPIO 时钟 ， 开启TIM 时钟
//2、GPIO 初始化 GPIO 上拉输入
//3、配置时机单元 让 CNT 计数器内部 时钟的驱动下 自增运行
//4、配置输入捕获单元 : 滤波器 输入极性 直连通道还是交叉通道 分频器
//5、选择从模式的触发 源 这里是 TI1FP1
//6、触发之后存在的操作 ， 触发从模式的 reset
//7. 调用 TIM_CMD 开启定时器

//需要读取频率时 ，就读取 CCR寄存器。然后计算 Fc/N

void INPUTCAPTURE_R_INIT(){

    // GPIO 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // TIM 初始化
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 选择 时基单元 (如果 不写 也是默认使用内部时钟)
    TIM_InternalClockConfig(TIM2);// 选择内部时钟 TIM2


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // 由于要对 信号 进行采样 判定， 所以 使用 内部时钟的频率 作为采样的 频率， 而 TIM_ClockDivision 就是 将采样频率分频 划分，采样次数越多 精度越高，但是 延时 增加
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;// 指定时钟划分频  TIM_CKD_DIV1 一分频，
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 计数 模式， 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 65536 -1 ;//ARR 计数器 重装 器的值； 要在  0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;//PSC  预分频器 的值; 这里目的 是 定时 一秒钟；  要在 0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;// 指定重复计数器值； 高级计数器 才有的 给 0
    //配置时机 单元
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);




    // 配置 输入 捕获
    // 配置 通道 1
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//  通道 1  向上，
    TIM_ICInitStruct.TIM_ICFilter = 0xF;// 捕获过滤 应该是 第一个 滤波边沿触发器
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;// 触发方式
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;// 分频 ： TIM_ICPSC_DIV1不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;// 交叉通道  直连通道

    TIM_ICInit(TIM2, &TIM_ICInitStruct);

    // 配置 通道 2
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//  通道 2  向下，
    TIM_ICInitStruct.TIM_ICFilter = 0xF;// 捕获过滤 应该是 第一个 滤波边沿触发器
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;// 触发方式
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;// 分频 ： TIM_ICPSC_DIV1不分频
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;// 交叉通道  直连通道

    TIM_ICInit(TIM2, &TIM_ICInitStruct);


    // 主从 模式配置 ，这里 TI1FP1 输出到 从模式 经过 选择器后 变成 TRGI ，TRGI 会触发 Reset 把 CNT 设置为 0


    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);

    // 配置从模式 要 执行 Reset
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);


    TIM_Cmd(TIM2, ENABLE);

}

// 频率 Fc/N
uint32_t getICFreq(){
    return 1 / (TIM_GetCapture1(TIM3) +  1);
}

//占空比 INT ; CCR2 / CCR1;
uint32_t getICDuty(){
    return (TIM_GetCapture2(TIM3) + 1) /  (TIM_GetCapture1(TIM3) + 1);
}
