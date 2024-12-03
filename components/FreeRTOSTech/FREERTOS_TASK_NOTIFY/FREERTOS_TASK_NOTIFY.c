//
// Created by kingnan on 2024/12/1.
//
// 任务 通知 实现 轻量级 计数 信号量

// 任务 1  通知 任务2
// 任务 通知 是  任务1 去传输 信息到 任务 2 的 notify 结构体


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TASK_NOTIFY.h"



//任务优先级
#define START_TASK_TASK_NOTIFY_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_TASK_NOTIFY_FUNCTION_uxStackDepth       640
#define vTASK1_TASK_NOTIFY_FUNCTION_uxPriority         2
#define vTASK2_TASK_NOTIFY_FUNCTION_uxStackDepth       640
#define vTASK2_TASK_NOTIFY_FUNCTION_uxPriority         2


//任务句柄
TaskHandle_t    START_TASK_HANDLER_TASK_NOTIFY;
TaskHandle_t    TASK1_HANDLER_TASK_NOTIFY;
TaskHandle_t    TASK2_HANDLER_TASK_NOTIFY;
TaskHandle_t    TASK3_HANDLER_TASK_NOTIFY;
TaskHandle_t    TASK4_HANDLER_TASK_NOTIFY;
TaskHandle_t    DELETE_HANDLER_TASK_NOTIFY;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_COUNT;//信号量 句柄
static SemaphoreHandle_t xSEMAPHORE_HANDLE_BIN;// 二进制 信号量 句柄

static int sum = 0;
static volatile int flagCalcEnd = 0;
void vTASK1_TASK_NOTIFY(void *pvParameters){
    //vTaskDelay(100);
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
            xTaskNotifyGive(TASK2_HANDLER_TASK_NOTIFY);
        }
        vTASK1_NUM++;
        // xSemaphoreGive(xSEMAPHORE_HANDLE_COUNT);// 信号量 的 give 函数
        printf("vTASK1_TASK_NOTIFY %d\r\n", vTASK1_NUM);
        OLED_ShowNum(1, 8, vTASK1_NUM, 5);

        vTaskDelete(NULL);//
    }
}


// 任务 2  接收 任务以 通知 的数据
void vTASK2_TASK_NOTIFY(void *pvParameters){
    printf("vTASK2_TASK_NOTIFY start \r\n");
    //vTaskDelay(1);
    int value;
    while (1) {

        // 接收 来自  task1  的 通知 分为 获取 多次 和 获取 一次
        int type  = 0;
        if(type == 0){
            // 第一个参数 xClearCountOnExit 为 pdTRUE 时， 退出之前 清零 ， 不会影响 拿到的值； 只能获取 1 次；
            value = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        } else {
            // 第一个参数 xClearCountOnExit 为 pdFALSE 时，退出之前 不清零 只 减一 ， 不会影响 拿到的值;可以 获取 10 次
            value = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
        }
        printf("vTASK2_TASK_NOTIFY value = %d\r\n", value);

    }
}





//开始任务任务函数
void START_TASK_TASK_NOTIFY(void *pvParameters)
{

    // // 创建 计数型 信号量
    // xSEMAPHORE_HANDLE_COUNT = xSemaphoreCreateCounting(10, 0);// 最大值 是 10， 初始值 是 0
    // // 创建 二进制 信号量
    // xSEMAPHORE_HANDLE_BIN = xSemaphoreCreateBinary();
    // xSemaphoreGive(xSEMAPHORE_HANDLE_BIN);// 二进制信号量 默认 创建 是 0 ，所以这里传入 1

    xTaskCreate(
            vTASK1_TASK_NOTIFY,
            "Task1",
            vTASK1_TASK_NOTIFY_FUNCTION_uxStackDepth,
            NULL,
            vTASK1_TASK_NOTIFY_FUNCTION_uxPriority,
            &TASK1_HANDLER_TASK_NOTIFY
    );

    xTaskCreate(
            (TaskFunction_t)                           vTASK2_TASK_NOTIFY, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_TASK_NOTIFY_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_TASK_NOTIFY_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_TASK_NOTIFY // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskStartScheduler();// 开启任务调度器;
}



void FREERTOS_TASK_NOTIFY_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    START_TASK_TASK_NOTIFY("pvParameters");
}



