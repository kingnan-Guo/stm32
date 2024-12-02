//
// Created by 90175 on 2024/11/26.
// 事件组 同步点 的使用

// 任务1
// 任务2
// 任务3
// 每个任务 要等到 其他任务 都做完 才 最终打印
// 三个任务 优先级 不同， 第一个执行的 是 任务 3

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_EVENT_GROUP_SYNC.h"





//任务优先级
#define START_TASK_EVENT_GROUP_SYNC_PRIO		             1
//任务堆栈大小
#define START_STK_SIZE 		                                 128

#define vTASK1_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth        640
#define vTASK1_EVENT_GROUP_SYNC_FUNCTION_uxPriority          1
#define vTASK2_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth        640
#define vTASK2_EVENT_GROUP_SYNC_FUNCTION_uxPriority          2
#define vTASK3_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth        640
#define vTASK3_EVENT_GROUP_SYNC_FUNCTION_uxPriority          3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_EVENT_GROUP_SYNC;
TaskHandle_t    TASK1_HANDLER_EVENT_GROUP_SYNC;
TaskHandle_t    TASK2_HANDLER_EVENT_GROUP_SYNC;
TaskHandle_t    TASK3_HANDLER_EVENT_GROUP_SYNC;

// 队列 的 句柄
static QueueHandle_t xQueue_Handle;// 创建 队列 的 句柄

// 事件组 句柄
static EventGroupHandle_t EVENT_GROUP_CALC;

#define ONE (1<<0)
#define TWO (1<<1)
#define THREE (1<<2)
#define ALL (ONE | TWO | THREE)




void vTASK1_EVENT_GROUP_SYNC(void *pvParameters){
    const TickType_t xDelay100ms = pdMS_TO_TICKS( 1000UL );
    int i = 0;
    while (1) {
        // 开始 做
        printf("%s doing %d \r\n", (char * ) pvParameters, i);
        // 我已经做好 ，要等 所有人 都做好
        xEventGroupSync(EVENT_GROUP_CALC, ONE, ALL, portMAX_DELAY);
        printf("%s Done %d  \r\n", (char * ) pvParameters, i++);
        vTaskDelay(xDelay100ms);
    }
}



void vTASK2_EVENT_GROUP_SYNC(void *pvParameters){
    const TickType_t xDelay100ms = pdMS_TO_TICKS( 1000UL );
    int i = 0;
    while (1) {
        // 开始 做
        printf("%s doing %d \r\n", (char * ) pvParameters, i);
        // 我已经做好 ，要等 所有人 都做好
        xEventGroupSync(EVENT_GROUP_CALC, TWO, ALL, portMAX_DELAY);
        printf("%s Done %d  \r\n", (char * ) pvParameters, i++);
        vTaskDelay(xDelay100ms);
    }
}


// 这时第一个 处理的任务
void vTASK3_EVENT_GROUP_SYNC(void *pvParameters){
    const TickType_t xDelay100ms = pdMS_TO_TICKS( 1000UL );
    int i = 0;
    while (1) {
        // 开始 做
        printf("%s doing %d \r\n", (char * ) pvParameters, i);
        // 我已经做好 ，要等 所有人 都做好
        xEventGroupSync(EVENT_GROUP_CALC, THREE, ALL, portMAX_DELAY);
        printf("%s Done %d  \r\n", (char * ) pvParameters, i++);
        vTaskDelay(xDelay100ms);
    }
}



//开始任务任务函数
void START_TASK_EVENT_GROUP_SYNC(void *pvParameters)
{

    //taskENTER_CRITICAL();           //进入临界区
    // 创建 事件组
    EVENT_GROUP_CALC = xEventGroupCreate();

    // 创建三个任务
    xTaskCreate(
            vTASK1_EVENT_GROUP_SYNC,
            "Task1",
            vTASK1_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth,
            "vTask1 ",
            vTASK1_EVENT_GROUP_SYNC_FUNCTION_uxPriority,
            &TASK1_HANDLER_EVENT_GROUP_SYNC
    );
    xTaskCreate(
            vTASK2_EVENT_GROUP_SYNC, // 任务函数
            "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK2_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask2 ",// 传递给任务函数的参数
            vTASK2_EVENT_GROUP_SYNC_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK2_HANDLER_EVENT_GROUP_SYNC // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_EVENT_GROUP_SYNC, // 任务函数
            "vTask3",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK3_EVENT_GROUP_SYNC_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask3 ",// 传递给任务函数的参数
            vTASK3_EVENT_GROUP_SYNC_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK3_HANDLER_EVENT_GROUP_SYNC // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskStartScheduler();// 开启任务调度器;

    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_EVENT_GROUP_SYNC_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    START_TASK_EVENT_GROUP_SYNC("pvParameters");
}
