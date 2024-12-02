#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "timer_r.h"
//#include "FREERTOS_QUEUE_SET.h"
//#include "FREERTOS_TEMP.h"
//#include "FREERTOS_QUEUE.h"
//#include "FREERTOS_MUTEX.h"
#include "FREERTOS_SEMAPHORE.h"
//#include "FREERTOS_MUTEX_PRIORITY_INVERSION.h"
//#include "FREERTOS_EVENT_GROUP.h"
//#include "FREERTOS_EVENT_GROUP_SYNC.h"


int main(void) {
    OLED_Init();
//    FREERTOS_MUTEX_MAIN();
    FREERTOS_SEMAPHORE_MAIN();
    while(1) {
    }
}
