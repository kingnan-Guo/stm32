#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "PWM_R.h"
#include "INPUTCAPTURE_R.h"

int main(void) {
    OLED_Init();
    OLED_ShowNum(1,1, 0, 5);
    while(1) {
    }
}
