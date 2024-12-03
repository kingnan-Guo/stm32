//
// Created by 90175 on 2024/11/26.
//

// 定时器 相关

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TIMER.h"



//任务优先级
#define START_TASK_TIMER_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_TIMER_uxStackDepth       512
#define vTASK1_TIMER_uxPriority         3
#define vTASK2_TIMER_uxStackDepth       512
#define vTASK2_TIMER_uxPriority         3


//任务句柄
TaskHandle_t    START_TASK_HANDLER_TIMER;
TaskHandle_t    TASK1_HANDLER_TIMER;
TaskHandle_t    TASK2_HANDLER_TIMER;
TaskHandle_t    DELETE_HANDLER_TIMER;


void vTASK1_TIMER(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        if(num == 5){
            printf(" delete task2\r\n");
            vTaskSuspend(TASK2_HANDLER_TIMER);// 挂起
        }
        if(num == 10){
            vTaskResume(TASK2_HANDLER_TIMER);// 重
        }
         printf(" vTASK1_TIMER =%d\r\n",  num);
        //printf(" vTASK1_TIMER\r\n");
        OLED_ShowString(1, 1, "task1 = ");
        OLED_ShowNum(1, 8, num, 5);
        num++;
        vTaskDelay(1000);
    }
}



void vTASK2_TIMER(void *pvParameters){
    char *pcTaskName;
    int16_t num = 0x00;
    const TickType_t xDelay5ms = pdMS_TO_TICKS( 5UL );
    pcTaskName = ( char * ) pvParameters;
    while (1) {
         printf(" vTASK2_TIMER =%d\r\n",  num);
        //printf(" vTASK2_TIMER\r\n");
        OLED_ShowString(2, 1, "task2 = ");
        OLED_ShowNum(2, 8 , num, 5);
        num++;
        //printf( pcTaskName ); //打印字符串
//        vTaskDelay(xDelay5ms); // Delay for 500 milliseconds
        vTaskDelay(1000);
    }
}



void vDELETE_TIMER(void *pvParameters){
    while (1) {
    }
}
//
//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}



//开始任务任务函数
void START_TASK_TIMER(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_TIMER,
            "Task1",
            vTASK1_TIMER_uxStackDepth,
            "Task1",
            vTASK1_TIMER_uxPriority,
            &TASK1_HANDLER_TIMER
    );
    xTaskCreate(
            vTASK2_TIMER, // 任务函数
            "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK2_TIMER_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask2",// 传递给任务函数的参数
            vTASK2_TIMER_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK2_HANDLER_TIMER // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskDelete(START_TASK_HANDLER_TIMER); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_TIMER_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_TIMER,
            "START_TASK_TIMER",
            START_STK_SIZE,
            NULL,
            START_TASK_TIMER_PRIO,
            &START_TASK_HANDLER_TIMER
    );

    vTaskStartScheduler();// 开启任务调度器;
}
