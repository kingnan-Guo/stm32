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





TaskHandle_t Task1_NVIC_Handler;
TaskHandle_t Task2_NVIC_Handler; // 存放任务句柄
TaskHandle_t del_NVIC_Handler;
TaskHandle_t interrupt_task_Handler;// 开启 关闭 中断的 任务的句柄


void vTask1Function_NVIC(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        printf("printf task1=%d\r\n",  num++);
//        OLED_ShowString(2, 1, "task1 = ");
//        OLED_ShowNum(2, 9, num++, 5);
        vTaskDelay(1000);
    }
}

#define vTask2F_uxStackDepth    128
#define vTask2F_uxPriority      3
void vTask2Function_NVIC(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        printf("printf task2=%d\r\n",  num++);
//        OLED_ShowString(3, 1, "task2 = ");
//        OLED_ShowNum(3, 9, num++, 5);
        vTaskDelay(1000); // Delay for 500 milliseconds
    }
}

void delFunction_NVIC(void *pvParameters){
}



void  interrupt_task(){

    static uint32_t num = 0;
    while (1){
//        printf("interrupt_task =%d \r\n", getTime2InterruptCount());
        OLED_ShowNum(3, 9, getTime2InterruptCount(), 5);
        if(num == 5){
            // 关闭 中断
            portDISABLE_INTERRUPTS();
            printf("close interrupt\r\n");
        }
        if(num == 10){
            // 重启 中断
            portENABLE_INTERRUPTS();
            printf("open interrupt\r\n");
        }
        num++;

        vTaskDelay(1000);
    }
}







//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		128
//任务句柄
TaskHandle_t StartTask_FreeRTOS_NVIC_Handler;
//开始任务任务函数
void start_task_FreeRTOS_NVIC(void *pvParameters)
{
    // taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTask1Function_NVIC,
            "Task1",
            128,
            NULL,
            3,
            &Task1_NVIC_Handler
    );
//    xTaskCreate(
//            (TaskFunction_t)                          vTask2Function_NVIC, // 任务函数
//            (char *  )                                   "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            vTask2F_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            // (uint32_t)                                         vTask2F_uxStackDepth,
//            (void *  )                               "free2 0",// 传递给任务函数的参数
//            (UBaseType_t)                               vTask2F_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                        &Task2_NVIC_Handler // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );
//    xTaskCreate(
//            delFunction_NVIC,
//            "delFunction",
//            128,
//            NULL,
//            2,
//            &del_NVIC_Handler
//    );

    // 中断 控制
    xTaskCreate(
            interrupt_task,
            "interrupt_task",
            128,
            NULL,
            2,
            &interrupt_task_Handler
    );


    vTaskDelete(StartTask_FreeRTOS_NVIC_Handler); //删除开始任务;  为什么执行完成要删除？？？
    // taskEXIT_CRITICAL();            //退出临界区
}

void FreeRTOS_NVIC_MAIN(){

    Timer_R_MAIN();// NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4 ;// 抢占 优先级 改成 4 或者 5;  优先级为 4 的 无法停止， 优先级为 5 的 开启停止
    Serial_Init();
    RetargetInit(USART1);

    xTaskCreate(start_task_FreeRTOS_NVIC, "start_task_FreeRTOS_NVIC", START_STK_SIZE, NULL, START_TASK_PRIO, &StartTask_FreeRTOS_NVIC_Handler);

    vTaskStartScheduler();// 开启任务调度器;
}

