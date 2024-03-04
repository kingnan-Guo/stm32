#include "stm32f10x.h" 
#include "delay.h"
#include "LED.h"
#include "key.h"

uint8_t KeyNumber;

int main(void) {
    LED_Init();
    Key_Init();
	while(1) {
		// project begin

        KeyNumber = Key_GetNum();
        if(KeyNumber == 1){
            LED1_Turn();
        }

//        LED1_ON();
//        Delay_ms(500);
//        LED1_OFF();
//        Delay_ms(500);

    }
}
