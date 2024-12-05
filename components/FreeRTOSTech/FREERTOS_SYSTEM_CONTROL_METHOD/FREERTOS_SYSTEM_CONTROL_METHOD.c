//
// Created by 90175 on 2024/11/26.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_SYSTEM_CONTROL_METHOD.h"



//任务优先级
#define START_TASK_CONTROL_METHOD_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_CONTROL_METHOD_FUNCTION_uxStackDepth       512
#define vTASK1_CONTROL_METHOD_FUNCTION_uxPriority         3
#define vTASK2_CONTROL_METHOD_FUNCTION_uxStackDepth       512
#define vTASK2_CONTROL_METHOD_FUNCTION_uxPriority         3
#define vTASK3_CONTROL_METHOD_FUNCTION_uxStackDepth       512
#define vTASK3_CONTROL_METHOD_FUNCTION_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_CONTROL_METHOD;
TaskHandle_t    TASK1_HANDLER_CONTROL_METHOD;
TaskHandle_t    TASK2_HANDLER_CONTROL_METHOD;
TaskHandle_t    TASK3_HANDLER_CONTROL_METHOD;

// 临界区域 ，本质上是 关闭中断； 在 FreeRTOS 中 挂起调度器 的时候 会应用  此临界区代码
// 会被关闭优先级低于   configMAX_SYSCALL_INTERRUPT_PRIORITY 的中断， 临界区要快进快出
void vTASK1_CONTROL_METHOD(void *pvParameters){

    taskENTER_CRITICAL();           //进入临界区

    taskEXIT_CRITICAL();            //退出临界区
    while (1) {

    }
}

//
void vTASK2_CONTROL_METHOD(void *pvParameters){
    taskYIELD();// 使用 taskYIELD 进行任务切换 ，那是不是可以在循环中添加这句 然后就跳出此 循环了，也有可能 重新进入此循环
    while (1) {

    }
}

void vTASK3_CONTROL_METHOD(void *pvParameters){
    vTaskStartScheduler();// 开启任务调度器
    vTaskEndScheduler(); // 关闭任务调度器， 不能随意调度
    while (1) {

    }
}





//开始任务任务函数
void START_TASK_CONTROL_METHOD(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_CONTROL_METHOD,
            "Task1",
            vTASK1_CONTROL_METHOD_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_CONTROL_METHOD_FUNCTION_uxPriority,
            &TASK1_HANDLER_CONTROL_METHOD
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_CONTROL_METHOD, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_CONTROL_METHOD_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_CONTROL_METHOD_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_CONTROL_METHOD // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_CONTROL_METHOD,
            "Task3",
            vTASK3_CONTROL_METHOD_FUNCTION_uxStackDepth,
            NULL,
            vTASK3_CONTROL_METHOD_FUNCTION_uxPriority,
            &TASK3_HANDLER_CONTROL_METHOD
    );
    vTaskDelete(START_TASK_HANDLER_CONTROL_METHOD); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_CONTROL_METHOD_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_CONTROL_METHOD,
            "START_TASK_CONTROL_METHOD",
            START_STK_SIZE,
            NULL,
            START_TASK_CONTROL_METHOD_PRIO,
            &START_TASK_HANDLER_CONTROL_METHOD
    );

    vTaskStartScheduler();// 开启任务调度器;
}
