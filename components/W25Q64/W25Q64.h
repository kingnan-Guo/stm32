#include "stm32f10x.h"
#include "coustomSPI.h"

/**
 * 初始化
 */
void W25Q64_Init(void){
    customSPI_Init();
}
