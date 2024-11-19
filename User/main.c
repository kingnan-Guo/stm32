#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "SOFTWARE_CONTROL_MPU6050.h"


int main(void) {
    OLED_Init();



    SOFTWARE_CONTROL_MAIN();


    while(1) {
    }
}
