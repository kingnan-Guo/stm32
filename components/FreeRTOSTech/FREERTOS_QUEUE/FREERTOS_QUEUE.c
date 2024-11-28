//
// Created by 90175 on 2024/11/28.
//






#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_QUEUE.h"



//任务优先级
#define START_TASK_QUEUE_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_QUEUE_FUNCTION_uxStackDepth      128
#define vTASK1_QUEUE_FUNCTION_uxPriority         3
#define vTASK2_QUEUE_FUNCTION_uxStackDepth      128
#define vTASK2_QUEUE_FUNCTION_uxPriority         3

#define vTASK3_QUEUE_FUNCTION_uxStackDepth      256
#define vTASK3_QUEUE_FUNCTION_uxPriority         2

#define vTASK4_QUEUE_FUNCTION_uxStackDepth      256
#define vTASK4_QUEUE_FUNCTION_uxPriority         2

//任务句柄
TaskHandle_t    START_TASK_HANDLER_QUEUE;
TaskHandle_t    TASK1_HANDLER_QUEUE;
TaskHandle_t    TASK2_HANDLER_QUEUE; // 存放任务句柄
TaskHandle_t    TASK3_HANDLER_QUEUE; // 存放任务句柄
TaskHandle_t    TASK4_HANDLER_QUEUE; // 存放任务句柄
TaskHandle_t    TASK5_HANDLER_QUEUE; // 存放任务句柄
TaskHandle_t    TASK6_HANDLER_QUEUE; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_QUEUE;


static QueueHandle_t xQueue_Handle;// 创建 队列 的 句柄
static QueueHandle_t XQUEUE_UART_LOCK_HANDLE;// 创建 串口 互斥的锁

void vTASK1_QUEUE(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        if(num == 5){
            printf(" delete task2\r\n");
            vTaskSuspend(TASK2_HANDLER_QUEUE);// 挂起
        }
        if(num == 10){
            vTaskResume(TASK2_HANDLER_QUEUE);// 重
        }
        // printf(" vTASK1_QUEUE =%d\r\n",  num);
        printf(" vTASK2_QUEUE\r\n");
        OLED_ShowString(1, 1, "task1 = ");
        OLED_ShowNum(1, 8, num, 5);
        num++;
        vTaskDelay(1000);
    }
}



void vTASK2_QUEUE(void *pvParameters){
    char *pcTaskName;
    int16_t num = 0x00;
    const TickType_t xDelay5ms = pdMS_TO_TICKS( 5UL );
    pcTaskName = ( char * ) pvParameters;
    while (1) {
        // printf(" vTASK2_QUEUE =%d\r\n",  num);
        printf(" vTASK2_QUEUE\r\n");
        OLED_ShowString(2, 1, "task2 = ");
        OLED_ShowNum(2, 8 , num, 5);
        num++;
        //printf( pcTaskName ); //打印字符串
        vTaskDelay(xDelay5ms); // Delay for 500 milliseconds
    }
}


static int vTASK3_QUEUE_NUM;
static int sum = 0;
static volatile int flagCalcEnd = 0;
void vTASK3_QUEUE(void *pvParameters){
    volatile int i = 0;

    flagCalcEnd = 0;
    //printf("%s\r\n", (char *)pvParameters);
     OLED_ShowString(2,1, (char *)pvParameters);

    while (1){
        for (i = 0; i < 1000; ++i) {
            sum++;
            OLED_ShowNum(1, 2, sum, 5);
        }
        xQueueSend(xQueue_Handle, &sum, portMAX_DELAY);// &sum 要把地址 发给 队列 ; portMAX_DELAY 一直等待
        // printf("vTASK3_QUEUE %d", vTASK3_QUEUE_NUM++);
        OLED_ShowNum(2, 8, vTASK3_QUEUE_NUM++, 5);
        sum = 0;
    }
}


uint16_t vTASK4_QUEUE_NUM;
void vTASK4_QUEUE(void *pvParameters){
    int vTASK4_QUEUE_VALUE;

    while (1){
        //添加标记
        flagCalcEnd = 0;
        // 读取队列 把值放到 vTASK4_QUEUE_VALUE
        xQueueReceive(xQueue_Handle, &vTASK4_QUEUE_VALUE, portMAX_DELAY);
        flagCalcEnd = 1;

        OLED_ShowString(3,1, (char *)pvParameters);
//        printf("vTASK3_QUEUE %d", vTASK4_QUEUE_NUM++);
        OLED_ShowNum(3, 8, vTASK4_QUEUE_NUM++, 5);
//        printf("vTASK3_QUEUE %d", vTASK4_QUEUE_NUM++);
        printf("%d", vTASK4_QUEUE_VALUE);

    }
}



// Lock 独占 互斥的 串口 锁  函数
int XQUEUE_UART_LOCK_INIT(){
    int val = 1;
    // 创建 队列
    XQUEUE_UART_LOCK_HANDLE = xQueueCreate(1, sizeof(int));
    if(XQUEUE_UART_LOCK_HANDLE == NULL){
        printf("can not create Queue lock");
        return -1;
    }

    // 写入一条数据
    xQueueSend(XQUEUE_UART_LOCK_HANDLE, &val, portMAX_DELAY);
    return  0;
}

// 读数据
void GET_UART_LOCK(){
    int value;
    xQueueReceive(XQUEUE_UART_LOCK_HANDLE, &value, portMAX_DELAY);
}

// 释放, 把使用权 释放掉
void PUT_UART_LOCK(){
    int value = 2;
    xQueueSend(XQUEUE_UART_LOCK_HANDLE, &value, portMAX_DELAY);
}

//
void vTASK5_QUEUE(void *pvParameters){
    OLED_ShowString(3,1, (char *)pvParameters);
    while (1){

        GET_UART_LOCK();
        //printf("%s\r\n", (char *)pvParameters);
        printf(" vTASK5_QUEUE \r\n");
        PUT_UART_LOCK();// 释放 锁
        vTaskDelay(1);// 主动释放， 让 任务 5 可以执行
    }
}
void vTASK6_QUEUE(void *pvParameters){
    OLED_ShowString(4,1, (char *)pvParameters);
    while (1){

        GET_UART_LOCK();
        //printf("%s\r\n", (char *)pvParameters);
        printf(" vTASK6_QUEUE \r\n");
        PUT_UART_LOCK();// 释放 锁
        vTaskDelay(1);// 主动释放， 让 任务 5 可以执行
    }
}





void vDELETE_QUEUE(void *pvParameters){
    while (1) {
    }
}





//开始任务任务函数
void START_TASK_QUEUE(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
//    xTaskCreate(
//            vTASK1_QUEUE,
//            "Task1",
//            vTASK1_QUEUE_FUNCTION_uxStackDepth,
//            NULL,
//            vTASK1_QUEUE_FUNCTION_uxPriority,
//            &TASK1_HANDLER_QUEUE
//    );
//    xTaskCreate(
//            (TaskFunction_t)                           vTASK2_QUEUE, // 任务函数
//            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            vTASK2_QUEUE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            // (uint32_t)                                         vTask2F_uxStackDepth,
//            (void *  )                                "free2 0",// 传递给任务函数的参数
//            (UBaseType_t)                                vTASK2_QUEUE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                         &TASK2_HANDLER_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );

    // 队列相关

    xTaskCreate(
            (TaskFunction_t)                           vTASK3_QUEUE, // 任务函数
            (char *  )                                    "vTask3",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK3_QUEUE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            (void *  )                                "vTask3 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK3_QUEUE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK3_HANDLER_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    xTaskCreate(
            (TaskFunction_t)                           vTASK4_QUEUE, // 任务函数
            (char *  )                                    "vTask4",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK4_QUEUE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            (void *  )                                "vTask4 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK4_QUEUE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK4_HANDLER_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );


//    // 互斥锁相关
//    xTaskCreate(
//            (TaskFunction_t)                           vTASK5_QUEUE, // 任务函数
//            (char *  )                                    "vTask5",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            256,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            (void *  )                                "free5 0",// 传递给任务函数的参数
//            (UBaseType_t)                                1,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                         &TASK5_HANDLER_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );
//    xTaskCreate(
//            (TaskFunction_t)                           vTASK6_QUEUE, // 任务函数
//            (char *  )                                    "vTask6",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            256,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            (void *  )                                "free6 0",// 传递给任务函数的参数
//            (UBaseType_t)                                1,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                         &TASK6_HANDLER_QUEUE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );



    vTaskDelete(START_TASK_HANDLER_QUEUE); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_QUEUE_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    // 创建 队列
    xQueue_Handle = xQueueCreate(2, sizeof(int));
    if(xQueue_Handle == NULL){
        printf("can not create Queue");
    }

//    int val = XQUEUE_UART_LOCK_INIT();// 初始化  互斥 相关队列

    xTaskCreate(
            START_TASK_QUEUE,
            "START_TASK_QUEUE",
            START_STK_SIZE,
            NULL,
            START_TASK_QUEUE_PRIO,
            &START_TASK_HANDLER_QUEUE
    );


    vTaskStartScheduler();// 开启任务调度器;
}

