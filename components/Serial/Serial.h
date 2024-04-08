#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArr(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Print(char *format, ...);
void USART1_IRQHandler(void);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxFlag(void);
#endif
