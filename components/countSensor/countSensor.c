//
// Created by kingnan on 2024/3/6.
//
/**
 * 1 配置 GPIO RCC开启 时钟
 * 2 AFIO 开启 时钟 （内核的外设nvic 不需要开启时钟， EXIT也不需要开启外设）
 * 3、执行完 GPIO_EXTILineConfig 后，AFIO 的第 15 个数据选择器 就配置好了
 *        其中输入端被  拨到了 GPIOA 上 对应引脚 PA14
 *
 * 4、配置 EXTI
 */
#include "stm32f10x.h"
uint16_t counsensor_count = 1;
void countSensor_Init(void){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 开启 AFIO 的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructre;
    GPIO_InitStructre.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructre.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructre);

    // AFIO
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

    EXTI_InitTypeDef EXTI_InitStruct;
    //指定 要配置的中断线
    EXTI_InitStruct.EXTI_Line = EXTI_Line15;
    // 指定 中断选择的新状态
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    // 外部中断 的模式 分为 中断 模式  事件模式
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//
    // 下降沿触法 EXTI_Trigger_Falling   上升沿触发EXTI_Trigger_Rising 上升下降沿触发 EXTI_Trigger_Rising_Falling
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStruct);

    // 配置 NVIC
    // 配置 中断 之前 指定 中断的分组
    // 然后初始化 一下

    // 配置 优先级分组 先转优先级 pre-emption priority（抢占优先级） 和 从占优先级 subpriority （响应优先级）
    // NVIC_PriorityGroup_2 两位抢占 两位响应

    // 分组的方式 整个芯片 只能用 一种， 所以这里的代码 整个 工程 只需要 执行一次 就可以
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//指定 中断 通道开启 或 关闭
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//指定 中断 通道 是 使能 还是  失能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;// 指定 所选通道的抢占 优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;// 指定 所选通道的  响应优先级
    NVIC_Init(&NVIC_InitStructure);


}

uint16_t getCount(void){

    return  counsensor_count;
}

//中断 函数  固定的名称
void EXTI15_10_IRQHandler(void) {
    //判定  中断 标志位 是否 为 1
    if(EXTI_GetITStatus(EXTI_Line15) == SET){
        // 执行 中断 程序 start
        counsensor_count++;
        //执行中断程序 end
        // 最后 清除中断 标志位， 如果不清除  会一直进入到中断程序 中
        EXTI_ClearITPendingBit(EXTI_Line15);

    }
}
