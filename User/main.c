#include "stm32f10x.h"
//#include "LED.h"
#include "delay.h"
#include "OLED.h"
#include "Timer.h"
//#include "key.h"
//#include "PWM.h"
//#include "IC.h"
#include "Encode.h"
//uint16_t number_timer_interrupt;
int8_t Speed;
int main(void) {
    Timer_Init();
    OLED_Init();
    Encode_Init();
    OLED_ShowString(1, 1, "CNT: ");
    OLED_ShowString(2, 1, "speed: ");
    OLED_ShowString(3, 1, "Time: ");
    while(1) {

        OLED_ShowSignedNum(1, 5, Encode_Get(),5);// 展示 计数

//        Speed = Encode_get_and_ClearCnt();
        OLED_ShowSignedNum(2, 7, Speed,5);// 展示 速度

        //延时 1000ms 模拟值闸门
        //Delay_ms(1000);
        OLED_ShowNum(4, 1, TIM_GetCounter(TIM2), 5);
    }
}

/**
 * 定时器 2 的中断 每秒执行一次
 */
void TIM2_IRQHandler(void){
    // 检测中断标志位 ; TIM_GetITStatus 获取中断标志位 ； TIM2 选择的时钟； TIM_IT_Update 哪种 中断方式
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        // 每隔一秒读取一下速度
        Speed = Encode_get_and_ClearCnt();
        // 清除 中断 标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

}
