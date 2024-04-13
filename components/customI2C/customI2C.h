#ifndef __CUSTOM_I2C_H
#define __CUSTOM_I2C_H




void customI2C_Init(void);
// 封装函数 用于拉低抬高电平
void customI2C_W_SCL(uint8_t BitValue);
void customI2C_W_SDA(uint8_t BitValue);
uint8_t customI2C_R_SDA(void);
void customI2C_Start(void);
void customI2C_Stop(void);
void customI2C_SendByte(uint8_t Byte);
uint8_t customI2C_ReceiveByte(void);
void customI2C_SendACK(uint8_t AckBit);
uint8_t customI2C_ReceiveAckByte(void);

#endif
