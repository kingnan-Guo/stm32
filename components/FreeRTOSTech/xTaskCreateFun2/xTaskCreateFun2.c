#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Serial.h"

#include "retarget.h"

//int16_t num = 0x00;
//int16_t num2 = 0x00;
//TaskHandle_t vTask2F_pxCreatedTask; // 存放任务句柄




// 定义两个任务的堆栈大小
#define TASK1_STACK_SIZE 128
#define TASK2_STACK_SIZE 128

// 定义任务优先级
#define TASK1_PRIORITY 2
#define TASK2_PRIORITY 1

// 定义串口输出的任务句柄
TaskHandle_t xTask1Handle;

// 任务1的函数
void vTask11(void *pvParameters)
{
    // 任务循环
    for (;;)
    {
        // 数据处理逻辑，这里简单地模拟了一个循环更改数据的过程
        // 可以根据实际需求修改
//        vTaskDelay(pdMS_TO_TICKS(1000)); // 假设每秒更改一次数据
        // 输出数据到串口
        // 这里假设输出的数据为 "Task 1: Data changed\r\n"
        printf("Task 1: Data changed\r\n");

        // 等待一段时间再进行下一轮循环
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// 任务2的函数
void vTask22(void *pvParameters)
{
    // 输出一次数据到串口
    // 这里假设输出的数据为 "Task 2: Initialized\r\n"
    printf("Task 2: Initialized\r\n");
//    vTaskDelete(NULL);
    // 任务2只输出一次，所以不需要进入循环
}


void xTaskCreateFun2Main(void ){
    Serial_Init();
    RetargetInit(USART1);

    // 创建任务1，使用xTaskCreate创建
    xTaskCreate(vTask11, "Task11", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, &xTask1Handle);

    // 创建任务2，使用xTaskCreate创建
    xTaskCreate(vTask22, "Task22", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, NULL);


    vTaskStartScheduler();// 开启任务调度器;
}


