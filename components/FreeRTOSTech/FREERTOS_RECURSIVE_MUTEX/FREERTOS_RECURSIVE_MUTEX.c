//
// Created by 90175 on 2024/12/02.
//
// 互斥量 递归锁


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_RECURSIVE_MUTEX.h"



//任务优先级
#define START_TASK_RECURSIVE_MUTEX_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_RECURSIVE_MUTEX_FUNCTION_uxStackDepth       512
#define vTASK1_RECURSIVE_MUTEX_FUNCTION_uxPriority         3
#define vTASK2_RECURSIVE_MUTEX_FUNCTION_uxStackDepth       512
#define vTASK2_RECURSIVE_MUTEX_FUNCTION_uxPriority         3


//任务句柄
TaskHandle_t    START_TASK_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK1_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK2_HANDLER_RECURSIVE_MUTEX; // 存放任务句柄
TaskHandle_t    TASK3_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK4_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    DELETE_HANDLER_RECURSIVE_MUTEX;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_RECURSIVE_MUTEX;// 互斥 信号量 句柄


void vTASK1_RECURSIVE_MUTEX(void *pvParameters){
    while (1) {

    }
}



void vTASK2_RECURSIVE_MUTEX(void *pvParameters){
    while (1) {
    }
}



void vDELETE_RECURSIVE_MUTEX(void *pvParameters){
    while (1) {
    }
}
//
//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}



// 任务 3  任务 4 共同使用的 函数； 互斥信号量   测试函数
void vTASK3_RECURSIVE_MUTEX(void *pvParameters){
    while (1) {
        // 阻塞 等待 知道 它 出现； 任务 3 和 任务 4 就是 互斥的， 只有一个 可以使用  print 串口
        // 这里是 互斥 信号量
        xSemaphoreTake(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, portMAX_DELAY);
        printf("%s\r\n", (char *)pvParameters);
        xSemaphoreGive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
    }
}



//开始任务任务函数
void START_TASK_RECURSIVE_MUTEX(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区


    // 创建 互斥信号量; 创建完互斥信号量 会 自动 添加值 1
    xSEMAPHORE_HANDLE_RECURSIVE_MUTEX = xSemaphoreCreateMutex();

    xTaskCreate(
            vTASK1_RECURSIVE_MUTEX,
            "Task1",
            vTASK1_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_RECURSIVE_MUTEX_FUNCTION_uxPriority,
            &TASK1_HANDLER_RECURSIVE_MUTEX
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_RECURSIVE_MUTEX, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_RECURSIVE_MUTEX_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_RECURSIVE_MUTEX // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );


    // 信号量  互斥
    xTaskCreate(
            vTASK3_RECURSIVE_MUTEX,
            "vTask3",
            512,
            "vTask3 param",
            2,
            &TASK3_HANDLER_RECURSIVE_MUTEX
    );
    xTaskCreate(
            vTASK3_RECURSIVE_MUTEX,
            "vTask4",
            512,
            "vTask4 param",
            2,
            &TASK4_HANDLER_RECURSIVE_MUTEX
    );


    vTaskDelete(START_TASK_HANDLER_RECURSIVE_MUTEX); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_RECURSIVE_MUTEX_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_RECURSIVE_MUTEX,
            "START_TASK_RECURSIVE_MUTEX",
            START_STK_SIZE,
            NULL,
            START_TASK_RECURSIVE_MUTEX_PRIO,
            &START_TASK_HANDLER_RECURSIVE_MUTEX
    );

    vTaskStartScheduler();// 开启任务调度器;
}
