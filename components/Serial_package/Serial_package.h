#ifndef __SERIAL_PACKAGE_H
#define __SERIAL_PACKAGE_H
#include <stdio.h>
extern uint8_t Serial_package_TxPacket[];//声明数组外部可调用
extern uint8_t Serial_package_RxPacket[];

void Serial_package_Init(void);
void Serial_package_SendByte(uint8_t Byte);
void Serial_package_SendArr(uint8_t *Array, uint16_t Length);
void Serial_package_SendString(char *String);
void Serial_package_SendNumber(uint32_t Number, uint8_t Length);
void Serial_package_Print(char *format, ...);
void USART1_IRQHandler(void);

uint8_t Serial_package_GetRxFlag(void);

void Serial_package_SendPacket(void);
#endif
