//
// Created by 90175 on 2024/11/26.
//
// 定时器 相关

//创建 两个任务
//任务 1 会 创建一个定时器，每隔 100ms 之后去打印信息
//任务 2 优先级跟守护任务不一样，看看 定时器 会不会被其他 优先级的任务打断


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
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
#define vTASK3_TIMER_uxStackDepth       512
#define vTASK3_TIMER_uxPriority         3
//任务句柄
TaskHandle_t    START_TASK_HANDLER_TIMER;
TaskHandle_t    TASK1_HANDLER_TIMER;
TaskHandle_t    TASK2_HANDLER_TIMER;
TaskHandle_t    DELETE_HANDLER_TIMER;

// 定时器 任务句柄
static TimerHandle_t xTIMER_HANDLER;

// 任务1  里 创建定时器 ，并且 启动
void vTASK1_TIMER(void *pvParameters){

    //启动定时器 xTIMER_HANDLER
    xTimerStart(xTIMER_HANDLER, 0);// 启动定时器, 启动定时器 的本质 是把启动定时器的命令发到 定时器命令队列，由守护任务来启动。这个队列又可能满，所以 有可能需要等到 ， 0 不等待
    while (1) {
        printf("vTASK1 ing %d\r\n");
    }
}

void vTASK2_TIMER(void *pvParameters){
    while (1) {
    }
}


// 定时器 回调
void myTimerCallbackFunction(TimerHandle_t xTimer){
    static  int count = 0;
    printf("myTimerCallbackFunction count = %d\r\n", count++);
}

//开始任务任务函数
    void START_TASK_TIMER(void *pvParameters)
    {


        // 返回值成功 返回 句柄否则返回 NULL
        xTIMER_HANDLER = xTimerCreate(
           "pcTimer",   // 名称
           100,     // 间隔周期 tick 为单位
           pdTRUE,       // pdTRUE 自动加载 pdFLASE 一次性
           NULL,            // 回调函数可使用此为参数，比如分辨是哪个定时器，也可以
           myTimerCallbackFunction  // 回调函数

        );


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
    vTaskStartScheduler();// 开启任务调度器;
}



void FREERTOS_TIMER_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
//    xTaskCreate(
//            START_TASK_TIMER,
//            "START_TASK_TIMER",
//            START_STK_SIZE,
//            NULL,
//            START_TASK_TIMER_PRIO,
//            &START_TASK_HANDLER_TIMER
//    );
//    vTaskStartScheduler();// 开启任务调度器;

    START_TASK_TIMER("pvParameters");
}
