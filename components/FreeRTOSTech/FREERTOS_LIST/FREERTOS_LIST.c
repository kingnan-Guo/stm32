//
// Created by 90175 on 2024/11/25.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_LIST.h"



//任务优先级
#define START_TASK_LIST_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		        128
//任务句柄
TaskHandle_t    START_TASK_HANDLER_LIST;

TaskHandle_t    TASK1_HANDLER_LIST;
TaskHandle_t    TASK2_HANDLER_LIST; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_LIST;


void vTASK1_LIST(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        if(num == 5){
            printf("printf delete task2\r\n");
            vTaskSuspend(TASK2_HANDLER_LIST);// 挂起
        }
        if(num == 10){
            vTaskResume(TASK2_HANDLER_LIST);// 重
        }
        printf("printf task1=%d\r\n",  num);
        OLED_ShowString(1, 1, "task1");
        OLED_ShowNum(2, 1, num, 5);
        num++;
        vTaskDelay(1000);
    }
}


#define vTask2F_uxStackDepth    128
#define vTask2F_uxPriority      5
void vTASK2_LIST(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        printf("printf test2\r\n");
        OLED_ShowString(3, 1, "task2 0");
        OLED_ShowNum(4, 1, num++, 5);
        vTaskDelay(1500); // Delay for 500 milliseconds
    }
}



void vDELETE_LIST(void *pvParameters){
    while (1) {
    }
}





//开始任务任务函数
void START_TASK_LIST(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_LIST,
            "Task1",
            128,
            NULL,
            3,
            &TASK1_HANDLER_LIST
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_LIST, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            128,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                3,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_LIST // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskDelete(START_TASK_HANDLER_LIST); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_LIST_INIT_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_LIST,
            "START_TASK_LIST",
            START_STK_SIZE,
            NULL,
            START_TASK_LIST_PRIO,
            &START_TASK_HANDLER_LIST
    );

    vTaskStartScheduler();// 开启任务调度器;
}
