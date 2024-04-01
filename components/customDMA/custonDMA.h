#ifndef __CUSTOMDMA_H
#define __CUSTOMDMA_H
void customDMA_Init(uint32_t AddrStar, uint32_t AddrEnd, uint16_t Size);
void customDMA_Transfer(void);
#endif
