#include "stm32f10x.h"

#include "OLED.h"
#include "Timer.h"
#include "ADC_DMA.h"


uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};


int main(void) {
    OLED_Init();
    AD_DMA_Init();

    OLED_ShowString(1, 1, "AD0");
    OLED_ShowString(2, 1, "AD1");
    OLED_ShowString(3, 1, "AD2");
//    OLED_ShowString(4, 1, "AD3");



    while(1) {

        OLED_ShowNum(1,5,AD_DMA_Value[0], 4);
        OLED_ShowNum(2,5,AD_DMA_Value[1], 4);
        OLED_ShowNum(3,5,AD_DMA_Value[2], 4);
//        OLED_ShowNum(4,5,AD_DMA_Value[4], 4);


    }
}


