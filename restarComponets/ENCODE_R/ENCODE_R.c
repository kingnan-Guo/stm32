//
// Created by 90175 on 2024/11/14.
//
// 1、开启 GPIOA TIM3 时钟
// 2、初始化 TIM3 的 CH1的 GPIOA_Pin_6  CH2 的 GPIOA_Pin_7
// 3、初始化 TIM3 ， 计数器 Arr 最大 65535 , 不分频 ， 但是 不开启 内部时钟计数，当前 计数器 CNT 由编码器托管
// 4、 TIM3CH1 TIM3CH2 先进入 滤波器 去噪声， 极性选择 ，这里是 直接联通，  然后 输出 TI1FP1 、TI2FP2
// 5、 配置 输入模式 类似输入捕获
// 5、TI1FP1 TI2FP2会输入到 编码器，需要编码器配置, 编码器需要配置 （计时器，编码器模式, CH1 极性， CH2 极性，）， 因为时计数 所以两个都是  向上 ，极性不反转
// 6、TIM_Cmd()
#include "stm32f10x.h"
#include "ENCODE_R.h"

int8_t Speed;
void ENCODE_R_INIT(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    // TIM 初始化


    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    // 由于要对 信号 进行采样 判定， 所以 使用 内部时钟的频率 作为采样的 频率， 而 TIM_ClockDivision 就是 将采样频率分频 划分，采样次数越多 精度越高，但是 延时 增加
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;// 指定时钟划分频  TIM_CKD_DIV1 一分频，
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;// 计数 模式， 向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 65536 -1 ;//ARR 计数器 重装 器的值； 要在  0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;//PSC  预分频器 的值; 这里目的 是 定时 一秒钟；  要在 0~ 65535 以内
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;// 指定重复计数器值； 高级计数器 才有的 给 0
    //配置时机 单元
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);



    // 配置 输入
    // 配置 通道 1
    TIM_ICInitTypeDef TIM_ICInitStruct;
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//  通道 1  向上，
    TIM_ICInitStruct.TIM_ICFilter = 0xF;// 捕获过滤 应该是 第一个 滤波边沿触发器
//    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;// 触发方式
//    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;// 分频 ： TIM_ICPSC_DIV1不分频

    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    // 配置 通道 2
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//  通道 2  向下，
    TIM_ICInitStruct.TIM_ICFilter = 0xF;// 捕获过滤 应该是 第一个 滤波边沿触发器
//    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;// 触发方式
//    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;// 分频 ： TIM_ICPSC_DIV1不分频
//    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;// 交叉通道
    TIM_ICInit(TIM3, &TIM_ICInitStruct);

    // 配置编码器  (计数器， 编码器模式， CH1 的极性，CH2 的极性)
    // TIM_EncoderMode_TI12 上下都 计数
    // 这里 配置 的 TIM_ICPolarity_Rising 会覆盖 上面的 TIM_ICInitStruct.TIM_ICPolarity 极性配置
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);


    TIM_Cmd(TIM3, ENABLE);


}


// 获取当前计数器的值 就是 获取以下 CNT 内值
// int16 在 65535 时 负数
int16_t ENCODE_R_Get_INC(){
    return TIM_GetCounter(TIM3);
}


//void ENCODE_R_ClearnCnt(){
//    TIM_SetCounter(TIM3, 0);
//}


int16_t LastTimeTIM3_INTC = 0;// 记录上一次的 CNT 的值
int16_t ENCODE_R_GET_Difference(){
    int16_t TEMP;
    int16_t TIM3_INTC = TIM_GetCounter(TIM3);

    TEMP = TIM3_INTC - LastTimeTIM3_INTC;// 当前值  和 上一次的 差值
    LastTimeTIM3_INTC = TIM3_INTC;

    return TEMP;
}

int16_t ENCODE_R_GET_Speed(){
    return Speed;
}


/**
 * 配合 TIM2 定时器中断才能使用， 要引入 Timer_r.h
 * 定时器 2 的中断 每秒执行一次
 */
void TIM2_IRQHandler(void){
    // 检测中断标志位 ; TIM_GetITStatus 获取中断标志位 ； TIM2 选择的时钟； TIM_IT_Update 哪种 中断方式
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        // 每隔一秒读取一下速度
//        Speed = ENCODE_R_GET_Difference();
        // 清除 中断 标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

}









// ==================

//#include "ENCODE_R.h"
//
//
//int main(void) {
//    OLED_Init();
//
//    ENCODE_R_INIT();
//    OLED_ShowNum(1,1, 0, 5);
//    while(1) {
//        OLED_ShowNum(2,1, ENCODE_R_Get_INC(), 5);
//        OLED_ShowNum(3,1, ENCODE_R_GET_Speed(), 5);
//    }
//}
