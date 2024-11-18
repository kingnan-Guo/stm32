#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "USART_R.h"

int main(void) {
    OLED_Init();

    main_test();


    OLED_ShowNum(2,5, 1, 5);
    while(1) {
    }
}
