#ifndef __CUSTOMSPI_H
#define __CUSTOMSPI_H
void customSPI_Init(void);
void customSPI_Start(void);
void customSPI_Stop(void);
uint8_t customSPI_SwapByte(uint8_t ByteSend);
uint8_t customSPI_SwapByte_2(uint8_t ByteSend);
#endif
