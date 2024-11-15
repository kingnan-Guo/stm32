//
// Created by 90175 on 2024/11/14.
//

#ifndef STM32F103C8T6PROJECT_DMA_R_H
#define STM32F103C8T6PROJECT_DMA_R_H
void DMA_R_INIT(uint32_t AddrStar, uint32_t AddrEnd, uint16_t size);
void DMA_R_TRANSFER();
void SHOW_DATA_INIT();
void SHOW_DATA_WHILE();
#endif //STM32F103C8T6PROJECT_DMA_R_H
