//
// Created by 90175 on 2024/11/25.
//


// FreeRTOS 可以 开启 禁止 中断， 这里展示了 如何禁止定时器中断， 只能 禁止 优先级 5 及5 以下的中断函数，
//感觉在这里 禁止了所有的 中断 函数，
// 停止 中断后 是不是其他的东西 都不可以 运行了，但是我想不是，停止 中断 是指的是 不再进入中断 函数吗，中断标志位也不会有变化？？？
// FreeRTOS任务 和 定时器任务 是如何 同时运行的？？？，这里主要是 定时器 外设 不会占用 CPU 的 运行，所以  禁止 中断 指的是 禁止外设？？


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FreeRTOS_NVIC.h"
#include "timer_r.h"
#include "delay.h"




//任务优先级
#define START_TASK_NVIC_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_NVIC_FUNCTION__uxStackDepth      128
#define vTASK1_NVIC_FUNCTION_uxPriority         3
#define vTASK2_NVIC_FUNCTION__uxStackDepth      128
#define vTASK2_NVIC_FUNCTION_uxPriority         3


//任务句柄
TaskHandle_t    START_TASK_HANDLER_NVIC;
TaskHandle_t    TASK1_HANDLER_NVIC;
TaskHandle_t    TASK2_HANDLER_NVIC; // 存放任务句柄
TaskHandle_t    TASK3_HANDLER_NVIC; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_NVIC;


void vTASK1_NVIC(void *pvParameters){
    int16_t num = 0;
    while (1) {
        printf("printf task1\r\n");
        //printf("printf task1=%d\r\n",  num);
        OLED_ShowString(1, 1, "task1 = ");
        OLED_ShowNum(1, 9, num, 5);
        num++;
        vTaskDelay(1000);
    }
}



void vTASK2_NVIC(void *pvParameters){
    int16_t num2 = 0x00;
    while (1) {
        printf("printf task2\r\n");
        // printf("printf task2=%d\r\n",  num2);
        OLED_ShowString(2, 1, "task2 = ");
        OLED_ShowNum(2, 9 , num2++, 5);
        vTaskDelay(1000); // Delay for 500 milliseconds
    }
}

void vTASK3_NVIC(void *pvParameters){
    char task_num = 0;
    while (1) {

        if(task_num == 5){
            // 关闭 中断
            portDISABLE_INTERRUPTS();
            printf("close interrupt\r\n");

        }
        if(task_num == 10){
            // 重启 中断
            portENABLE_INTERRUPTS();
            printf("open interrupt\r\n");
        }
        task_num++;

        OLED_ShowNum(3,1,getTime2InterruptCount(), 5);
        OLED_ShowNum(4,1,TIM_GetCounter(TIM2), 5);
        vTaskDelay(1000);
    }
}



void vDELETE_NVIC(void *pvParameters){
    while (1) {
    }
}





//开始任务任务函数
void START_TASK_NVIC(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_NVIC,
            "Task1",
            128,
            NULL,
            3,
            &TASK1_HANDLER_NVIC
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_NVIC, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            128,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                4,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_NVIC // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_NVIC,
            "Task3",
            128,
            NULL,
            5,
            &TASK3_HANDLER_NVIC
    );
    vTaskDelete(START_TASK_HANDLER_NVIC); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_NVIC_MAIN(){
    Timer_R_MAIN();
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_NVIC,
            "START_TASK_NVIC",
            START_STK_SIZE,
            NULL,
            START_TASK_NVIC_PRIO,
            &START_TASK_HANDLER_NVIC
    );

    vTaskStartScheduler();// 开启任务调度器;
}

