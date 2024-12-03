//
// Created by 90175 on 2024/11/26.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_INTERRUPT.h"


#define START_TASK_INTERRUPT_PRIO		            1   //任务优先级
#define START_STK_SIZE 		                        128 //任务堆栈大小

#define vTASK1_INTERRUPT_FUNCTION_uxStackDepth       512
#define vTASK1_INTERRUPT_FUNCTION_uxPriority         3
#define vTASK2_INTERRUPT_FUNCTION_uxStackDepth       512
#define vTASK2_INTERRUPT_FUNCTION_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_INTERRUPT;
TaskHandle_t    TASK1_HANDLER_INTERRUPT;
TaskHandle_t    TASK2_HANDLER_INTERRUPT;

void vTASK1_INTERRUPT(void *pvParameters){
    while (1) {
    }
}



void vTASK2_INTERRUPT(void *pvParameters){
    while (1) {

    }
}


//开始任务任务函数
void START_TASK_INTERRUPT(void *pvParameters)
{
    xTaskCreate(
            vTASK1_INTERRUPT,
            "Task1",
            vTASK1_INTERRUPT_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_INTERRUPT_FUNCTION_uxPriority,
            &TASK1_HANDLER_INTERRUPT
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_INTERRUPT, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_INTERRUPT_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_INTERRUPT_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_INTERRUPT // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskStartScheduler();// 开启任务调度器;
}



void FREERTOS_INTERRUPT_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    START_TASK_INTERRUPT("pvParameters");
}
