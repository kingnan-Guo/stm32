//
// Created by 90175 on 2024/11/7.
//
//1. APB2 RCC 开启时钟 初始化 pin
//2. AFIO
//3. EXTI 初始化,
//4. NVIC 配置, NVIC 是 与CPU 再一起 不需要 开启时钟

#include "stm32f10x.h"
#include "customizeInterrupt.h"
#include "OLED.h"
uint16_t InterruptCount = 0;
void Interrupt_Init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 开启 AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //
    GPIO_EXTILineConfig(RCC_APB2Periph_GPIOB, GPIO_PinSource5);

    // 开启  EXTI
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line5;//
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;// 使能
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;// 中断
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;// 下降沿 触发
    EXTI_Init(&EXTI_InitStruct);

    // 配置 NVIC

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 中断 分组； 先占优先级（抢占） 和 从占有先机（相应优先级）

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;// 指定通道; 在stm32f10x.h文件中，找到 对应的通道，因为 要使用 pin 由于10-15 被合并成同一个通道，所以 选择 EXTI15_10_IRQn
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;// 指定中断通道 使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;// 抢占 优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;// 响应 优先级
    NVIC_Init(&NVIC_InitStruct);

}

uint16_t getNum(){
    return  InterruptCount;
}

void addInterruptCount(){
    InterruptCount++;
}



// 中断函数; 中断函数的名字都是固定的 ,每一个 中断通道 中断对应一个 中断函数,我的理解是  这个 中断发生后会到固定的地址去 找到函数 调用函数, 固定地址里可能储存的是 要调用 函数的地址
// 在启动文件(Startup/startup_stm32f103c8tx.s)中 的 中断向量表中 找到 相应的函数 handler 就是 中断 函数的名称; EXTI15_10_IRQHandler
//无参数 无返回值
//void EXTI15_10_IRQHandler(void){
//    // 在中断中 有两个 函数 可以在中断中执行 在 stm32f10x_exti.h 中
//    // EXTI_GetITStatus(EXTI_Line15);//（中断） 在中断 函数里如果想查看标志位 ， 获取重点标志位是否被置 1了
//    if(EXTI_GetITStatus(EXTI_Line15) == SET){
//        // 执行一些 程序
//        InterruptCount++;
//        // 中断结束后 都要 清除 中断标志位 ,要不然  一直 卡在中断中
//        // EXTI_ClearITPendingBit(uint32_t EXTI_Line);//（中断） 清除 中断 挂起 标志位
//        EXTI_ClearITPendingBit(EXTI_Line15);
//    }
//
//}


void ____EXTI9_5_IRQHandler(void) {
    //判定  中断 标志位 是否 为 1
    if(EXTI_GetITStatus(EXTI_Line5) == SET){
        // 执行 中断 程序 start
        InterruptCount++;
        //执行中断程序 end
        // 最后 清除中断 标志位， 如果不清除  会一直进入到中断程序 中
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}



void Interrupt_MAIN(){
    OLED_Init();
    Interrupt_Init();
    OLED_ShowNum(1, 1,  1, 5);

    while (1){
        OLED_ShowNum(3, 1,  getNum(), 5);
    }
}
void Interrupt_WHILE(){
    OLED_ShowNum(3, 1,  getNum(), 5);
}

