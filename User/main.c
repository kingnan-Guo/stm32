#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "USART_R.h"
#include "SOFTWARE_I2C.h"

int main(void) {
    OLED_Init();

//    main_test();
    SOFTWARE_I2C_main();
    while(1) {
    }
}
