#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "ENCODE_R.h"


int main(void) {
    OLED_Init();

    ENCODE_R_INIT();
    OLED_ShowNum(1,1, 0, 5);
    while(1) {
        OLED_ShowNum(2,1, ENCODE_Get_INC(), 5);
        OLED_ShowNum(3,1, ENCODE_R_GET_Speed(), 5);
    }
}
