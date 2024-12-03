#include "stm32f10x.h"
#include "OLED.h"
#include "delay.h"
#include <stdio.h>
#include "timer_r.h"

//#include "FREERTOS_MUTEX.h"
//#include "FREERTOS_SEMAPHORE.h"
//#include "FREERTOS_MUTEX_PRIORITY_INVERSION.h"
//#include "FREERTOS_RECURSIVE_MUTEX.h"
//#include "FREERTOS_EVENT_GROUP.h"
//#include "FREERTOS_EVENT_GROUP_SYNC.h"
//#include "FREERTOS_TASK_NOTIFY.h"
//#include "FREERTOS_TASK_NOTIFY_QUEUE.h"
//#include "FREERTOS_TASK_NOYIFY_EVENT_GROUP.h"
//#include "FREERTOS_TIMER.h"
#include "FREERTOS_TIMER_DEBOUNCE.h"

//#include "EXTI_Interrupt.h"

//#include "countSensor.h"

#include "customizeInterrupt.h"
int main(void) {
    OLED_Init();
//    OLED_ShowNum(1,1,2,5);
//    FREERTOS_MUTEX_MAIN();
//    FREERTOS_SEMAPHORE_MAIN();
//    FREERTOS_MUTEX_PRIORITY_INVERSION_MAIN();
//    FREERTOS_RECURSIVE_MUTEX_MAIN();
//    FREERTOS_EVENT_GROUP_MAIN();
//    FREERTOS_EVENT_GROUP_SYNC_MAIN();
//    FREERTOS_TASK_NOTIFY_MAIN();
//    FREERTOS_TASK_NOTIFY_QUEUE_MAIN();

//    FREERTOS_TASK_NOYIFY_EVENT_GROUP_MAIN();
//    FREERTOS_TIMER_MAIN();

    FREERTOS_TIMER_DEBOUNCE_MAIN();
//    countSensor_Init_MAIN();


//    EXTI_Interrup_R_MAIN();
//    Interrupt_MAIN();


}
