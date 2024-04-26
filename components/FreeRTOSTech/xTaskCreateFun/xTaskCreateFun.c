#include "stm32f10x.h"
#include "OLED.h"
//#include "FreeRTOS.h"
//#include "task.h"

void vTask1Function(void *pvParameters){

}

void xTaskCreateFunMain(void ){
    OLED_ShowString(2, 1, "xTaskCreateFunMain");
//    TaskHandle_t xHandleTask1;
//
//    xTaskCreate(vTask1Function, "Task1", 100, NULL, 1, &xHandleTask1);
}