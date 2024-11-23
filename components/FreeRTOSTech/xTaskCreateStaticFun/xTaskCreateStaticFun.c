//
// Created by 90175 on 2024/11/23.
//


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "xTaskCreateStaticFun.h"

#define STATIC_START_STK_SIZE               128
#define vStartStaticTask_uxPriority         1
// 定义开始任务 的任务句柄
TaskHandle_t StartStaticTask_Handler;
// 开始 任务 的 任务堆栈
StackType_t StaticStartTaskStack[STATIC_START_STK_SIZE];
StaticTask_t StaticStartTaskTCB;// 任务 控制块

#define STATIC_STK1_SIZE               128
#define vStaticTask1_uxPriority         2
TaskHandle_t StaticTask_Handler1;// 定义 任务 的任务句柄
// 开始 任务 的 任务堆栈
StackType_t StaticTaskStack1[STATIC_START_STK_SIZE];
StaticTask_t StaticTaskTCB1;// 任务 控制块

#define STATIC_STK2_SIZE               128
#define vStaticTask2_uxPriority         3
TaskHandle_t StaticTask_Handler2;// 定义 任务 的任务句柄
// 开始 任务 的 任务堆栈
StackType_t StaticTaskStack2[STATIC_START_STK_SIZE];
StaticTask_t StaticTaskTCB2;// 任务 控制块




void static_task1(void * pvParameters){
    int16_t num = 0x00;
    while (1) {
        if(num == 5){
            vTaskDelete(StaticTask_Handler2);
            printf("printf delete task2\r\n");
        }
        printf("printf task1 =%d\r\n",  num++);

        OLED_ShowNum(3, 1, num, 5);
        vTaskDelay(1000);

    }
}
void static_task2(void * pvParameters){
    int num = 0;
    while (1) {

        OLED_ShowString(1, 1, "static_task2");
        OLED_ShowNum(2, 1, num++, 5);
        vTaskDelay(2000);
    }
}

void static_start_task(void * pvParameters){


    StaticTask_Handler1 = xTaskCreateStatic(
        (TaskFunction_t)                         static_task1,//任务 函数
        (const char *)                              "static_task1",// 函数 名称
        (const configSTACK_DEPTH_TYPE)          STATIC_STK1_SIZE,// 堆栈大小
        (void *)                                NULL, // 传递给任务函数的参数
        (UBaseType_t)                              vStaticTask1_uxPriority,// 任务优先级;  不能设置 0 级 和 32 级 因为好像记得 有其他函数调用
        (StackType_t *)                        StaticTaskStack1,// 任务 堆栈
        (StaticTask_t *)                         &StaticTaskTCB1 // 任务控制块
    );

    StaticTask_Handler2 = xTaskCreateStatic(
        (TaskFunction_t)                         static_task2,//任务 函数
        (const char *)                              "static_task2",// 函数 名称
        (const configSTACK_DEPTH_TYPE)          STATIC_STK2_SIZE,// 堆栈大小
        (void *)                                "params", // 传递给任务函数的参数
        (UBaseType_t)                              vStaticTask2_uxPriority,// 任务优先级;  不能设置 0 级 和 32 级 因为好像记得 有其他函数调用
        (StackType_t *)                        StaticTaskStack2,// 任务 堆栈
        (StaticTask_t *)                         &StaticTaskTCB2 // 任务控制块
    );


    vTaskDelete(StartStaticTask_Handler);// 删除 开始任务 ； 运行一次就结束




}


void xTaskCreateStaticFun_Main(){
    Serial_Init();
    RetargetInit(USART1);

    StartStaticTask_Handler = xTaskCreateStatic(
            (TaskFunction_t)                         static_start_task,//任务 函数
            (const char *)                              "static_start_task",// 函数 名称
            (const configSTACK_DEPTH_TYPE)          STATIC_START_STK_SIZE,// 堆栈大小
            (void *)                                NULL, // 传递给任务函数的参数
            (UBaseType_t)                              vStartStaticTask_uxPriority,// 任务优先级;  不能设置 0 级 和 32 级 因为好像记得 有其他函数调用
            (StackType_t *)                        StaticStartTaskStack,// 任务 堆栈
            (StaticTask_t *)                        &StaticStartTaskTCB // 任务控制块
    );

    vTaskStartScheduler();// 开启任务调度器;
}

























// 如果 打包没有这两个 函数 定义 那么 要自己 实现
//// 定义 空闲 任务 控制 块
//static StaticTask_t IdleTaskTCB;
////任务堆栈
//static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
//
//// 空闲任务 所需内存
//void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
//                                    StackType_t ** ppxIdleTaskStackBuffer,
//                                    configSTACK_DEPTH_TYPE * puxIdleTaskStackSize ){
//    * ppxIdleTaskTCBBuffer = &IdleTaskTCB;
//    * ppxIdleTaskStackBuffer = IdleTaskStack;
//    * puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;// 任务堆栈大小
//};

//void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
//                                    StackType_t ** ppxIdleTaskStackBuffer,
//                                    configSTACK_DEPTH_TYPE * puxIdleTaskStackSize ){
//    * ppxIdleTaskTCBBuffer = &IdleTaskTCB;
//    * ppxIdleTaskStackBuffer = IdleTaskStack;
//    * puxIdleTaskStackSize = configTIMER_TASK_STACK_DEPTH;// 任务堆栈大小
//};


