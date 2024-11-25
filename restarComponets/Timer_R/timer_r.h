//
// Created by 90175 on 2024/11/11.
//

#ifndef STM32F103C8T6PROJECT_TIMER_R_H
#define STM32F103C8T6PROJECT_TIMER_R_H
void Timer_R_Init();
uint16_t getTime2InterruptCount();

void Timer_R_MAIN(void);
void Timer_R_MAIN_WHILE(void);
#endif //STM32F103C8T6PROJECT_TIMER_R_H
