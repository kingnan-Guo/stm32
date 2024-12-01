//
// Created by kingnan on 2024/12/1.
//
// 信号量
// 计数




#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_SEMAPHORE.h"



//任务优先级
#define START_TASK_SEMAPHORE_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_SEMAPHORE_FUNCTION_uxStackDepth       512
#define vTASK1_SEMAPHORE_FUNCTION_uxPriority         3
#define vTASK2_SEMAPHORE_FUNCTION_uxStackDepth       512
#define vTASK2_SEMAPHORE_FUNCTION_uxPriority         3


//任务句柄
TaskHandle_t    START_TASK_HANDLER_SEMAPHORE;
TaskHandle_t    TASK1_HANDLER_SEMAPHORE;
TaskHandle_t    TASK2_HANDLER_SEMAPHORE; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_SEMAPHORE;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_COUNT;//信号量 句柄


static int sum = 0;

void vTASK1_SEMAPHORE(void *pvParameters){
    volatile int i = 0;
    uint16_t vTASK3_QUEUE_NUM;
    //printf("%s\r\n", (char *)pvParameters);
    OLED_ShowString(1,1, (char *)pvParameters);
    while (1){
        for (i = 0; i < 1000; ++i) {
            sum++;
            OLED_ShowNum(1, 2, sum, 5);
        }
        xSemaphoreGive(xSEMAPHORE_HANDLE_COUNT);// 信号量 的 give 函数
        printf("vTASK3_QUEUE %d\r\n", vTASK3_QUEUE_NUM++);
        OLED_ShowNum(1, 8, i++, 5);
        sum = 0;
    }
}



void vTASK2_SEMAPHORE(void *pvParameters){
    while (1) {
        // 阻塞 等待 知道 它 出现； 任务 1 释放 xSemaphoreGive ， vTASK2_SEMAPHORE 会被唤醒
        xSemaphoreTake(xSEMAPHORE_HANDLE_COUNT, portMAX_DELAY);
    }
}



void vDELETE_SEMAPHORE(void *pvParameters){
    while (1) {
    }
}
//
//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}



//开始任务任务函数
void START_TASK_SEMAPHORE(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区

    // 创建 计数型 信号量
    xSEMAPHORE_HANDLE_COUNT = xSemaphoreCreateCounting(10, 0);// 最大值 是 10， 初始值 是 0


    xTaskCreate(
            vTASK1_SEMAPHORE,
            "Task1",
            vTASK1_SEMAPHORE_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_SEMAPHORE_FUNCTION_uxPriority,
            &TASK1_HANDLER_SEMAPHORE
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_SEMAPHORE, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_SEMAPHORE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_SEMAPHORE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_SEMAPHORE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskDelete(START_TASK_HANDLER_SEMAPHORE); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_SEMAPHORE_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_SEMAPHORE,
            "START_TASK_SEMAPHORE",
            START_STK_SIZE,
            NULL,
            START_TASK_SEMAPHORE_PRIO,
            &START_TASK_HANDLER_SEMAPHORE
    );

    vTaskStartScheduler();// 开启任务调度器;
}
