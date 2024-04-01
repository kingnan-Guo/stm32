#include "stm32f10x.h"

#include "OLED.h"
#include "AD.h"
#include "custonDMA.h"
#include "Timer.h"

uint16_t SRAMValue = 0xaa;// 0x20
const uint16_t ROMValue = 0x33;// 地址 0x80
uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};


int main(void) {
    // DataA 不是变量；是数组名 数组名就是地址，所以不需要加地址符号

    OLED_Init();
    // 如果不直接改变DataA； 整个过程 DataA 是不会变化的
    customDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);

    OLED_ShowString(1, 1, "DataA");
    OLED_ShowString(3, 1, "DataB");

    OLED_ShowHexNum(1, 8,  (uint32_t)DataA, 8);
    OLED_ShowHexNum(3, 8,  (uint32_t)DataB, 8);
    //OLED_ShowHexNum(1, 1, SRAMValue, 2);




    //OLED_ShowHexNum(2, 1, (uint32_t)&ADValue, 8);
    //OLED_ShowHexNum(3, 1, (uint32_t)&ADValue2, 8);
    //OLED_ShowHexNum(4, 1, (uint32_t)&ADValue3, 8);
    while(1) {
        DataA[0] ++;
        DataA[1] ++;
        DataA[2] ++;
        DataA[3] ++;


        OLED_ShowHexNum(2, 1, DataA[0], 2);
        OLED_ShowHexNum(2, 4, DataA[1], 2);
        OLED_ShowHexNum(2, 7, DataA[2], 2);
        OLED_ShowHexNum(2, 10, DataA[3], 2);

        OLED_ShowHexNum(4, 1, DataB[0], 2);
        OLED_ShowHexNum(4, 4, DataB[1], 2);
        OLED_ShowHexNum(4, 7, DataB[2], 2);
        OLED_ShowHexNum(4, 10, DataB[3], 2);
        Delay_ms(1000);
        customDMA_Transfer();

        //OLED_ShowHexNum(3, 1, DataB[0], 2);
        //OLED_ShowHexNum(3, 4, DataB[1], 2);
        //OLED_ShowHexNum(3, 7, DataB[2], 2);
        //OLED_ShowHexNum(3, 10, DataB[3], 2);


        OLED_ShowHexNum(2, 1, DataA[0], 2);
        OLED_ShowHexNum(2, 4, DataA[1], 2);
        OLED_ShowHexNum(2, 7, DataA[2], 2);
        OLED_ShowHexNum(2, 10, DataA[3], 2);

        OLED_ShowHexNum(4, 1, DataB[0], 2);
        OLED_ShowHexNum(4, 4, DataB[1], 2);
        OLED_ShowHexNum(4, 7, DataB[2], 2);
        OLED_ShowHexNum(4, 10, DataB[3], 2);


        Delay_ms(1000);

    }
}


