#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"
#include "OLED.h"
//#include "countSensor.h"
#include "Timer.h"

uint16_t number_timer_interrupt = 0;
int64_t birth = 19930620;

int main(void) {

    OLED_Init();
//    OLED_ShowChar(1, 1, 'Y');
//    OLED_ShowChar(1, 2, 'e');
//    OLED_ShowString(1, 4, "miaomiao");
//    OLED_ShowNum(2, 1, birth,8);
//    OLED_ShowSignedNum(3, 1, -88, 2);
//    OLED_ShowHexNum(3, 5, 0xAABB, 4);
//    OLED_ShowBinNum(4, 1, 0xABAB, 16); // 十六 进制 转 二进制
//    Delay_ms(2000);
//    OLED_Clear();
    LED_Init();
    Timer_Init();
    OLED_ShowString(2, 1, "CNT");
	while(1) {




        OLED_ShowNum(2, 5,  Timer_GetCounter(), 5);

        OLED_ShowNum(3, 1, number_timer_interrupt, 8);
//        OLED_ShowNum(4, 1, TIM_GetCounter(TIM2), 5);

    }
}


//定时器中断 函数
//在启动 文件里找 TIM2_IRQHandler ; 当定时器产生更新中断时 那么就会执行这个函数
void TIM2_IRQHandler(void){
    // 检测中断标志位 ; TIM_GetITStatus 获取中断标志位 ； TIM2 选择的时钟； TIM_IT_Update 哪种 中断方式
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        number_timer_interrupt ++;
        LED1_Turn();
        // 清除 中断 标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }

}