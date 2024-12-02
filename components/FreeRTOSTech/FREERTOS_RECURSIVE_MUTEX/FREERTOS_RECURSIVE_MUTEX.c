//
// Created by 90175 on 2024/12/02.
//
// 互斥量 递归锁
// 互斥量 的本意 是 谁 持有 就 由 谁 去 释放
// 但是 freeRTOS 并没有 实现此功能


// 执行 过程
//


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
#define vTASK3_RECURSIVE_MUTEX_FUNCTION_uxStackDepth       512
#define vTASK3_RECURSIVE_MUTEX_FUNCTION_uxPriority         3
#define vTASK4_RECURSIVE_MUTEX_FUNCTION_uxStackDepth       512
#define vTASK4_RECURSIVE_MUTEX_FUNCTION_uxPriority         3
#define vTASK5_RECURSIVE_MUTEX_FUNCTION_uxStackDepth       512
#define vTASK5_RECURSIVE_MUTEX_FUNCTION_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK1_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK2_HANDLER_RECURSIVE_MUTEX; // 存放任务句柄
TaskHandle_t    TASK3_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK4_HANDLER_RECURSIVE_MUTEX;
TaskHandle_t    TASK5_HANDLER_RECURSIVE_MUTEX;

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

//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}

// 任务 3  任务 4 共同使用的 函数； 互斥信号量   测试函数
void vTASK3_RECURSIVE_MUTEX(void *pvParameters){
    while (1) {

        // 非  递归锁 会出问题 =============== 会被 其他 函数 直接 解锁 =======================
        // // 阻塞 等待 知道 它 出现； 任务 3 和 任务 4 就是 互斥的， 只有一个 可以使用  print 串口
        // // 这里是 互斥 信号量
        // xSemaphoreTake(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, portMAX_DELAY);
        // printf("%s\r\n", (char *)pvParameters);
        // xSemaphoreGive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
        // vTaskDelay(1);



        // 递归锁  === === 谁 持有 谁 才能 解锁 ===============
        xSemaphoreTakeRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, portMAX_DELAY);
        printf(" hold %s\r\n", (char *)pvParameters);// 持有中

        for (int i = 0; i < 10; ++i) {
            xSemaphoreTakeRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, portMAX_DELAY); // 再加锁
            printf("loop    %s\r\n", (char *)pvParameters, i);
            xSemaphoreGiveRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX); // 解锁
        }

        xSemaphoreGiveRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
        vTaskDelay(1);
    }
}


// 任务 3  任务 4  执行的过程中  任务 5 执行，
void vTASK5_RECURSIVE_MUTEX(void *pvParameters){
    vTaskDelay(1);
    while (1) {
        // 非  递归锁 会出问题 =============== ========================
        // while (1){
        //     // 如果 拿不到
        //     if(xSemaphoreTake(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, 0) != pdTRUE){
        //         // 直接  释放
        //         xSemaphoreGive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
        //     } else {
        //         break;
        //     }
        // }
        // xSemaphoreGive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
        // printf("%s\r\n", (char *)pvParameters);
        // vTaskDelay(1);


        // 使用 递归锁 ============== ======================
        // 谁 能获取到 持有 谁才可以解锁
        while (1){
            // 如果 拿不到
            if(xSemaphoreTakeRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX, 0) != pdTRUE){
                // 直接  释放
                xSemaphoreGiveRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
            } else {
                break;
            }
        }
        xSemaphoreGiveRecursive(xSEMAPHORE_HANDLE_RECURSIVE_MUTEX);
        printf("%s\r\n", (char *)pvParameters);
        vTaskDelay(1);

    }
}

//开始任务任务函数
void START_TASK_RECURSIVE_MUTEX(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区




    // 创建  可以递归 互斥的 信号量;
    xSEMAPHORE_HANDLE_RECURSIVE_MUTEX = xSemaphoreCreateRecursiveMutex();

//    xTaskCreate(
//            vTASK1_RECURSIVE_MUTEX,
//            "Task1",
//            vTASK1_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,
//            NULL,
//            vTASK1_RECURSIVE_MUTEX_FUNCTION_uxPriority,
//            &TASK1_HANDLER_RECURSIVE_MUTEX
//    );
//    xTaskCreate(
//            (TaskFunction_t)                           vTASK2_RECURSIVE_MUTEX, // 任务函数
//            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            vTASK2_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            // (uint32_t)                                         vTask2F_uxStackDepth,
//            (void *  )                                "free2 0",// 传递给任务函数的参数
//            (UBaseType_t)                                vTASK2_RECURSIVE_MUTEX_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                         &TASK2_HANDLER_RECURSIVE_MUTEX // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );


    // 信号量  互斥
    xTaskCreate(
            vTASK3_RECURSIVE_MUTEX,
            "vTask3",
            vTASK3_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,
            "vTask3 param",
            vTASK3_RECURSIVE_MUTEX_FUNCTION_uxPriority,
            &TASK3_HANDLER_RECURSIVE_MUTEX
    );
    xTaskCreate(
            vTASK3_RECURSIVE_MUTEX,
            "vTask4",
            vTASK4_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,
            "vTask4 param",
            vTASK4_RECURSIVE_MUTEX_FUNCTION_uxPriority,
            &TASK4_HANDLER_RECURSIVE_MUTEX
    );
    xTaskCreate(
            vTASK5_RECURSIVE_MUTEX,
            "vTask5",
            vTASK5_RECURSIVE_MUTEX_FUNCTION_uxStackDepth,
            "vTask5 param",
            vTASK5_RECURSIVE_MUTEX_FUNCTION_uxPriority,
            &TASK5_HANDLER_RECURSIVE_MUTEX
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
