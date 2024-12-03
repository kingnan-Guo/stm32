//
// Created by kingnan on 2024/12/1.
//
// 任务 通知 实现 轻量级 队列

// 任务 1 通知 任务2



#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TASK_NOTIFY_QUEUE.h"



//任务优先级
#define START_TASK_TASK_NOTIFY_QUEUE_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_TASK_NOTIFY_QUEUE_FUNCTION_uxStackDepth       640
#define vTASK1_TASK_NOTIFY_QUEUE_FUNCTION_uxPriority         2
#define vTASK2_TASK_NOTIFY_QUEUE_FUNCTION_uxStackDepth       640
#define vTASK2_TASK_NOTIFY_QUEUE_FUNCTION_uxPriority         2


//任务句柄
TaskHandle_t    START_TASK_HANDLER_TASK_NOTIFY_QUEUE;
TaskHandle_t    TASK1_HANDLER_TASK_NOTIFY_QUEUE;
TaskHandle_t    TASK2_HANDLER_TASK_NOTIFY_QUEUE;
TaskHandle_t    TASK3_HANDLER_TASK_NOTIFY_QUEUE;
TaskHandle_t    TASK4_HANDLER_TASK_NOTIFY_QUEUE;
TaskHandle_t    DELETE_HANDLER_TASK_NOTIFY_QUEUE;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_COUNT;//信号量 句柄
static SemaphoreHandle_t xSEMAPHORE_HANDLE_BIN;// 二进制 信号量 句柄

static int sum = 0;
static volatile int flagCalcEnd = 0;
void vTASK1_TASK_NOTIFY_QUEUE(void *pvParameters){
    uint16_t vTASK1_NUM;
    //printf("%s\r\n", (char *)pvParameters);
    OLED_ShowString(1,1, (char *)pvParameters);
    while (1){
        for (int i = 0; i < 1000; ++i) {
            sum++;
            OLED_ShowNum(1, 1, sum, 5);
        }
        // 通知 TASK2  10次  会从 1 累加到 10
        for (int j = 0; j < 10; ++j) {

            int type  = 0;
            // xTaskToNotify : 要给谁传送数据, ulValue: 发送数据的 值, eAction : eSetValueWithOverwrite 覆盖 ， eSetValueWithoutOverwrite 不覆盖
            if(type == 0){
                // 不覆盖 task2 得到的值是 1
                xTaskNotify(TASK2_HANDLER_TASK_NOTIFY_QUEUE, j, eSetValueWithoutOverwrite);
            } else {
                // 覆盖 后 得到的值 是 10
                xTaskNotify(TASK2_HANDLER_TASK_NOTIFY_QUEUE, j, eSetValueWithOverwrite);
            }

        }
        vTASK1_NUM++;

        printf("vTASK1_TASK_NOTIFY_QUEUE %d\r\n", vTASK1_NUM);
        OLED_ShowNum(1, 8, vTASK1_NUM, 5);
        vTaskDelete(NULL);//
    }
}


// 任务 2  接收 任务以 通知 的数据
void vTASK2_TASK_NOTIFY_QUEUE(void *pvParameters){
    printf("vTASK2_TASK_NOTIFY_QUEUE start \r\n");
    //vTaskDelay(1);
    uint32_t value;
    while (1) {

        // 接收 来自  task1  的 通知
        int type  = 0;
        if(type == 0){
            // ulBitsToClearOnEntry: 调这个函数 是否要清除 某一位 0 不需要,
            // ulBitsToClearOnExit:  退出 的时候 要不要清除 某一位 ， 0 不需要
            // pulNotificationValue: 获取的值
            // xTicksToWait: 等待多久
            xTaskNotifyWait(0, 0, &value, portMAX_DELAY);
        } else {

        }
        printf("vTASK2_TASK_NOTIFY_QUEUE value = %d\r\n", value);

    }
}





//开始任务任务函数
void START_TASK_TASK_NOTIFY_QUEUE(void *pvParameters)
{

    xTaskCreate(
            vTASK1_TASK_NOTIFY_QUEUE,
            "Task1",
            vTASK1_TASK_NOTIFY_QUEUE_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_TASK_NOTIFY_QUEUE_FUNCTION_uxPriority,
            &TASK1_HANDLER_TASK_NOTIFY_QUEUE
    );

    xTaskCreate(
            (TaskFunction_t)                           vTASK2_TASK_NOTIFY_QUEUE, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_TASK_NOTIFY_QUEUE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_TASK_NOTIFY_QUEUE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_TASK_NOTIFY_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskStartScheduler();// 开启任务调度器;
}



void FREERTOS_TASK_NOTIFY_QUEUE_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    START_TASK_TASK_NOTIFY_QUEUE("pvParameters");
}



