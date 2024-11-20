#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "SOFTWARE_CONTROL_W25Q64.h"


int main(void) {
    OLED_Init();



    SOFTWARE_CONTROL_W25Q64_MAIN();


    while(1) {
    }
}
