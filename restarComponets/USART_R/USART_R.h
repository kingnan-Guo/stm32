//
// Created by 90175 on 2024/11/15.
//

#ifndef STM32F103C8T6PROJECT_USART_R_H
#define STM32F103C8T6PROJECT_USART_R_H


void USART_R_INIT();
void USART_R_SEND_BYTE(uint8_t byte);
void USART_R_SEND_ARRAY(uint8_t * Array, uint16_t Length);
void USART_R_SEND_STRING(char *string);
void USART_R_SEND_NUMBER(uint32_t Number, uint8_t Length);
uint8_t GET_RECEIVE_DATA();
uint8_t GET_USART_R_RXFLAG();
void USART_R_Printf(char *format, ...);
void main_test();

#endif //STM32F103C8T6PROJECT_USART_R_H
