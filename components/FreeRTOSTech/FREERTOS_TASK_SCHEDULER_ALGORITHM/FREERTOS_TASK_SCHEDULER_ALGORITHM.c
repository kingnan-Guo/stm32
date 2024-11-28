//
// Created by 90175 on 2024/11/26.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TASK_SCHEDULER_ALGORITHM.h"



//任务优先级
#define START_TASK_TASK_SCHEDULER_ALGORITHM_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                                        128

#define vTASK1_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxStackDepth      128
#define vTASK1_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxPriority         3
#define vTASK2_TASK_SCHEDULER_ALGORITHM_FUNCTION__uxStackDepth      128
#define vTASK2_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxPriority         3


//任务句柄
TaskHandle_t    START_TASK_HANDLER_TASK_SCHEDULER_ALGORITHM;
TaskHandle_t    TASK1_HANDLER_TASK_SCHEDULER_ALGORITHM;
TaskHandle_t    TASK2_HANDLER_TASK_SCHEDULER_ALGORITHM; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_TASK_SCHEDULER_ALGORITHM;


void vTASK1_TASK_SCHEDULER_ALGORITHM(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        // printf(" vTASK1_TASK_SCHEDULER_ALGORITHM =%d\r\n",  num);
        printf(" vTASK2_TASK_SCHEDULER_ALGORITHM\r\n");
        OLED_ShowString(1, 1, "task1 = ");
        OLED_ShowNum(1, 8, num, 5);
        num++;
        vTaskDelay(1000);
    }
}



void vTASK2_TASK_SCHEDULER_ALGORITHM(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        // printf(" vTASK2_TASK_SCHEDULER_ALGORITHM =%d\r\n",  num);
        printf(" vTASK2_TASK_SCHEDULER_ALGORITHM\r\n");
        OLED_ShowString(3, 1, "task2 = ");
        OLED_ShowNum(3, 8 , num++, 5);
        vTaskDelay(1500); // Delay for 500 milliseconds
    }
}



void vDELETE_TASK_SCHEDULER_ALGORITHM(void *pvParameters){
    while (1) {
    }
}





//开始任务任务函数
void START_TASK_TASK_SCHEDULER_ALGORITHM(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_TASK_SCHEDULER_ALGORITHM,
            "Task1",
            vTASK1_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxPriority,
            &TASK1_HANDLER_TASK_SCHEDULER_ALGORITHM
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_TASK_SCHEDULER_ALGORITHM, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_TASK_SCHEDULER_ALGORITHM_FUNCTION__uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_TASK_SCHEDULER_ALGORITHM_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_TASK_SCHEDULER_ALGORITHM // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskDelete(START_TASK_HANDLER_TASK_SCHEDULER_ALGORITHM); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_TASK_SCHEDULER_ALGORITHM_INIT_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_TASK_SCHEDULER_ALGORITHM,
            "START_TASK_TASK_SCHEDULER_ALGORITHM",
            START_STK_SIZE,
            NULL,
            START_TASK_TASK_SCHEDULER_ALGORITHM_PRIO,
            &START_TASK_HANDLER_TASK_SCHEDULER_ALGORITHM
    );

    vTaskStartScheduler();// 开启任务调度器;
}
