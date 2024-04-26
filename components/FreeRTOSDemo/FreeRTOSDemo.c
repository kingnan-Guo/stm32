/**
 * 创建 FreeRTOS 任务
 */
#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"


void vTask1(void *pvParameters) {
    for (;;) {
        OLED_ShowString(3, 1, "FreeRtos 0");
        vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500 milliseconds
        OLED_ShowString(3, 1, "FreeRtos 1");
        vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 500 milliseconds
    }
}

void vTask2(void *pvParameters) {
    for (;;) {
        // Toggle LED2
        OLED_ShowString(4, 1, "FreeRtos 2");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1000 milliseconds
        OLED_ShowString(4, 1, "FreeRtos 3");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1000 milliseconds
    }
}

void FreeRTOSDemoMain(void) {
    // Create tasks
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Start FreeRTOS scheduler
    vTaskStartScheduler();
};
