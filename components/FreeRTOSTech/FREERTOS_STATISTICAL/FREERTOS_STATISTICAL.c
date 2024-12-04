//
// Created by 90175 on 2024/11/26.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_STATISTICAL.h"
#include "EXTI_Interrupt.h"



//任务优先级
#define START_TASK_STATISTICAL_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_STATISTICAL_FUNCTION_uxStackDepth       1024
#define vTASK1_STATISTICAL_FUNCTION_uxPriority         3
#define vTASK2_STATISTICAL_FUNCTION_uxStackDepth       1024
#define vTASK2_STATISTICAL_FUNCTION_uxPriority         3
//任务句柄
TaskHandle_t    START_TASK_HANDLER_STATISTICAL;
TaskHandle_t    TASK1_HANDLER_STATISTICAL;
TaskHandle_t    TASK2_HANDLER_STATISTICAL;


char  pcWriteBuffer[200];
void vTASK1_STATISTICAL(void *pvParameters){

    while (1) {
        vTaskList(pcWriteBuffer);
        //vTaskGetRunTimeStats(pcWriteBuffer);
        printf(pcWriteBuffer);
        vTaskDelay(1000);
    }
}
char pcWriteBuffer2[200];
void vTASK2_STATISTICAL(void *pvParameters){

    while (1) {
        vTaskGetRunTimeStats(pcWriteBuffer2);
        printf(pcWriteBuffer2);
        vTaskDelay(1000);
    }
}

//开始任务任务函数
void START_TASK_STATISTICAL(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_STATISTICAL,
            "Task1",
            vTASK1_STATISTICAL_FUNCTION_uxStackDepth,
            "Task2",
            vTASK1_STATISTICAL_FUNCTION_uxPriority,
            &TASK1_HANDLER_STATISTICAL
    );
    xTaskCreate(
            vTASK2_STATISTICAL,
            "Task2",
            vTASK2_STATISTICAL_FUNCTION_uxStackDepth,
            "Task2",
            vTASK2_STATISTICAL_FUNCTION_uxPriority,
            &TASK2_HANDLER_STATISTICAL
    );

    vTaskDelete(START_TASK_HANDLER_STATISTICAL); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}

void FREERTOS_STATISTICAL_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_STATISTICAL,
            "START_TASK_STATISTICAL",
            START_STK_SIZE,
            NULL,
            START_TASK_STATISTICAL_PRIO,
            &START_TASK_HANDLER_STATISTICAL
    );
    vTaskStartScheduler();// 开启任务调度器;
}
