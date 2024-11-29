//
// Created by 90175 on 2024/11/29.
//

// 队列集

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "queue.h"
#include "FREERTOS_QUEUE_SET.h"

#define START_TASK_QUEUE_SET_PRIO		            1 //任务优先级
#define START_STK_SIZE 		                        128 //任务堆栈大小

#define vTASK1_QUEUE_SET_FUNCTION_uxStackDepth      512
#define vTASK1_QUEUE_SET_FUNCTION_uxPriority        2
#define vTASK2_QUEUE_SET_FUNCTION_uxStackDepth      512
#define vTASK2_QUEUE_SET_FUNCTION_uxPriority        2
#define vTASK3_QUEUE_SET_FUNCTION_uxStackDepth      512
#define vTASK3_QUEUE_SET_FUNCTION_uxPriority        2

//任务句柄
TaskHandle_t    START_TASK_HANDLER_QUEUE_SET;
TaskHandle_t    TASK1_HANDLER_QUEUE_SET;
TaskHandle_t    TASK2_HANDLER_QUEUE_SET;
TaskHandle_t    TASK3_HANDLER_QUEUE_SET;
TaskHandle_t    DELETE_HANDLER_QUEUE_SET;

// 创建 队列 的 句柄
static QueueHandle_t xQUEUE1_HANDLE;
static QueueHandle_t xQUEUE2_HANDLE;

// 创建队列集 句柄
static QueueSetHandle_t xQUEUE_SET_HANDLE;

// 将数据存放到 队列 1
void vTASK1_QUEUE_SET(void *pvParameters){

    int i = 0;
    while (1) {
        i++;
        xQueueSend(xQUEUE1_HANDLE, &i, portMAX_DELAY);
//        printf("Task 1 \r\n");
        vTaskDelay(500);
    }
}

// 将数据存放到 队列 2
void vTASK2_QUEUE_SET(void *pvParameters){
    int i = 0;
    while (1) {
        i--;
        xQueueSend(xQUEUE2_HANDLE, &i, portMAX_DELAY);
//        printf("Task 2 \r\n");
        vTaskDelay(500);

    }
}

// 从队列集 中 读取数据
void vTASK3_QUEUE_SET(void *pvParameters){
    QueueSetMemberHandle_t handle;
    int i;
    while (1) {
        handle = xQueueSelectFromSet(xQUEUE_SET_HANDLE, portMAX_DELAY);
        if (handle == xQUEUE1_HANDLE || handle == xQUEUE2_HANDLE) {
            xQueueReceive(handle, &i, portMAX_DELAY);
            printf("Task 3: %d\r\n", i);

        }
    }
}


void vApplicationMallocFailedHook(void) {
    printf("内存分配失败！\r\n");
    while (1); // 卡住，方便调试
}

//开始任务任务函数
void START_TASK_QUEUE_SET(void *pvParameters)
{

    // 创建两个队列
    xQUEUE1_HANDLE = xQueueCreate(1, sizeof(int));
    if(xQUEUE1_HANDLE == NULL){
        printf("can not create Queue 1");
    }
    xQUEUE2_HANDLE = xQueueCreate(1, sizeof(int));
    if(xQUEUE2_HANDLE == NULL){
        printf("can not create Queue 2");
    }
    // 传参 是 队列容量， 4 是因为  每个队列里有两个 item
    xQUEUE_SET_HANDLE = xQueueCreateSet(2);
    if(xQUEUE_SET_HANDLE == NULL){
        printf("can not create Queue Set");
    }

    // 把两个 queue 他添加到 queue set
    xQueueAddToSet(xQUEUE1_HANDLE, xQUEUE_SET_HANDLE);
    xQueueAddToSet(xQUEUE2_HANDLE, xQUEUE_SET_HANDLE);



    // 创建 3 个任务


    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_QUEUE_SET,
            "Task1",
            vTASK1_QUEUE_SET_FUNCTION_uxStackDepth,
            "Task1",
            vTASK1_QUEUE_SET_FUNCTION_uxPriority,
            &TASK1_HANDLER_QUEUE_SET
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_QUEUE_SET, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_QUEUE_SET_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_QUEUE_SET_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_QUEUE_SET // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_QUEUE_SET,
            "Task3",
            vTASK3_QUEUE_SET_FUNCTION_uxStackDepth,
            "Task3",
            vTASK3_QUEUE_SET_FUNCTION_uxPriority,
            &TASK3_HANDLER_QUEUE_SET
    );
    vTaskDelete(START_TASK_HANDLER_QUEUE_SET); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}

void FREERTOS_QUEUE_SET_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    xTaskCreate(
            START_TASK_QUEUE_SET,
            "START_TASK_QUEUE_SET",
            START_STK_SIZE,
            NULL,
            START_TASK_QUEUE_SET_PRIO,
            &START_TASK_HANDLER_QUEUE_SET
    );
    vTaskStartScheduler();// 开启任务调度器;
}
