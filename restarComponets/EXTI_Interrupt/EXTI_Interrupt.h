//
// Created by kingnan on 2024/11/12.
//

#ifndef STM32F103C8T6PROJECT_EXTI_INTERRUPT_H
#define STM32F103C8T6PROJECT_EXTI_INTERRUPT_H
void EXTI_Interrup_R_Init();
uint16_t getExtiInterruptCount();
uint16_t getTIM2Count();
void EXTI_Interrup_R_MAIN();
void summationCount();//  累加 extiInterruptCount
#endif //STM32F103C8T6PROJECT_EXTI_INTERRUPT_H
