#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"

void vTask1Function(void *pvParameters){
    OLED_ShowString(2, 1, "xTaskCreateFunMain");
}

void xTaskCreateFunMain(void ){

    TaskHandle_t xHandleTask1;
    xTaskCreate(vTask1Function, "Task1", 100, NULL, 1, &xHandleTask1);
    vTaskStartScheduler();
}