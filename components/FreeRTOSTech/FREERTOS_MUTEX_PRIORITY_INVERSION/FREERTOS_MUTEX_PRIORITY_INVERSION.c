//
// Created by 90175 on 2024/12/02.
//
// 互斥量 优先级反转
// 解决 优先级反转 方法 是  优先级继承

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_MUTEX_PRIORITY_INVERSION.h"



//任务优先级
#define START_TASK_MUTEX_PRIORITY_INVERSION_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                                        128

#define vTASK1_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth       512
#define vTASK1_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority         1
#define vTASK2_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth       512
#define vTASK2_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority         2
#define vTASK3_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth       512
#define vTASK3_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_MUTEX_PRIORITY_INVERSION;
TaskHandle_t    TASK1_HANDLER_MUTEX_PRIORITY_INVERSION;
TaskHandle_t    TASK2_HANDLER_MUTEX_PRIORITY_INVERSION;
TaskHandle_t    TASK3_HANDLER_MUTEX_PRIORITY_INVERSION;
TaskHandle_t    TASK4_HANDLER_MUTEX_PRIORITY_INVERSION;
TaskHandle_t    DELETE_HANDLER_MUTEX_PRIORITY_INVERSION;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION;// 互斥 信号量  优先级反转


// 低优先级 任务
static int sum = 0;
void vTASK1_MUTEX_PRIORITY_INVERSION(void *pvParameters){
    int16_t num = 0x00;
    volatile int i = 0;

    printf(" vTASK1_MUTEX_PRIORITY_INVERSION start\r\n");
    while (1) {
        // 获得 互斥信号量 二进制信号
        xSemaphoreTake(xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION, portMAX_DELAY);


        printf(" vTASK1_MUTEX_PRIORITY_INVERSION long time task\r\n");
        // 耗时久 的任务
        for (i = 0; i < 1000; ++i) {
            OLED_ShowNum(1, 2, sum, 5);
        }
        printf(" vTASK2_TEMP =%d\r\n",  num++);

        // 释放
        xSemaphoreGive(xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION);
    }
}


// 中 优先级任务
void vTASK2_MUTEX_PRIORITY_INVERSION(void *pvParameters){
    const TickType_t xDelay30ms = pdMS_TO_TICKS( 30UL );
    printf(" vTASK2_MUTEX_PRIORITY_INVERSION start\r\n");
    vTaskDelay(xDelay30ms);
    while (1) {
    }
}






// 任务 3  优先级最高的 任务
void vTASK3_MUTEX_PRIORITY_INVERSION(void *pvParameters){
    const TickType_t xDelay10ms = pdMS_TO_TICKS( 10UL );

    printf(" vTASK3_MUTEX_PRIORITY_INVERSION start\r\n");
    vTaskDelay(xDelay10ms);// 这时会切换任务
    while (1) {
        printf(" vTASK3_MUTEX_PRIORITY_INVERSION wating for Lock\r\n");
        // 等待
        // 获取 信号量
        xSemaphoreTake(xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION, portMAX_DELAY);
        // 处理任务

        // 放开锁
       // xSemaphoreGive(xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION);
    }
}


//void vDELETE_MUTEX_PRIORITY_INVERSION(void *pvParameters){
//    while (1) {
//    }
//}

//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}


//开始任务任务函数
void START_TASK_MUTEX_PRIORITY_INVERSION(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    int a = 1; // a = 1 时  是有问题的代码
    if(a == 1){

        // 优先级反转 的运行过程

        // 1、task3  先执行， printf 后 进入 vtaskDelay(10) 等待 10 ms
        // 2、task2  执行， printf 后 进入 vtaskDelay(30) 等待 30 ms
        // 3、task1  执行， xSemaphoreTake 拿到 锁 执行 for 循环 1000 ，但是 这时 会在 for循环的过程中 突然 task3 的 vtaskDelay(10) 10ms 时间到了, 会被 task3 抢占
        // 4、task3  继续执行，这时 经过 printf 后 遇到了 xSemaphoreTake ，但是 这个值 当前 被 task1 占用，所以 task3 进入到 阻塞状态
        // 5、由于 task1 并没有执行完成，所以 继续执行 for 循环， 这时 task2 的 vtaskDelay(30) 30ms 到了，所以 被 task2 抢占
        // 6、task2 一直 执行



        // 创建 二进制 斥信号; 会导致 优先级反转
        xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION = xSemaphoreCreateBinary();
        // 赋值
        xSemaphoreGive(xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION);

    } else {

        // 优先级 继承 的 运行 过程
        // 1、task3  先执行， printf 后 进入 vtaskDelay(10) 等待 10 ms
        // 2、task2  执行， printf 后 进入 vtaskDelay(30) 等待 30 ms
        // 3、task1  执行， xSemaphoreTake 拿到 锁 执行 for 循环 1000 ，但是 这时 会在 for循环的过程中 突然 task3 的 vtaskDelay(10) 10ms 时间到了, 会被 task3 抢占
        // 4、task3  继续执行，这时 经过 printf 后 遇到了 xSemaphoreTake ，但是 这个值 当前 被 task1 占用，所以 task3 进入到 阻塞状态
        // 5、由于 task1 并没有执行完成，所以 继续执行 for 循环， 这时 task2 的 vtaskDelay(30) 30ms 到了， 但是这次并没有 被 task2 的抢占 , 即便 task2 的优先级更高
        // 6、这时  task1 继承了 task3 的优先级 变成了 3， 开始进行 执行 task1， 等到 for 循环 1000 次 以后， 释放了任务  xSemaphoreGive ，优先级 由 3 变成了 1
        // 7、task3 从阻塞状态 变成 执行状态，  task3 拿到了 锁 开始 执行

        // 8、 task3 执行完任务  ，然后 释放锁， 这时 是 执行 task2 还是  继续执行  task3，，我觉得 大概率执行 task3



        // 解决 优先级 反转的  方法 是 优先级 继承
        xSEMAPHORE_HANDLE_MUTEX_PRIORITY_INVERSION = xSemaphoreCreateMutex();
    }





    // 创建 三个 优先级的任务
    xTaskCreate(
            vTASK1_MUTEX_PRIORITY_INVERSION,
            "vTask1",
            vTASK1_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth,
            "vTask1 low",
            vTASK1_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority,
            &TASK1_HANDLER_MUTEX_PRIORITY_INVERSION
    );
    xTaskCreate(
            vTASK2_MUTEX_PRIORITY_INVERSION, // 任务函数
             "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK2_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask2 middle",// 传递给任务函数的参数
            vTASK2_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK2_HANDLER_MUTEX_PRIORITY_INVERSION // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_MUTEX_PRIORITY_INVERSION,
            "vTask3",
            vTASK3_MUTEX_PRIORITY_INVERSION_FUNCTION_uxStackDepth,
            "vTask3 heigth",
            vTASK3_MUTEX_PRIORITY_INVERSION_FUNCTION_uxPriority,
            &TASK3_HANDLER_MUTEX_PRIORITY_INVERSION
    );


    vTaskDelete(START_TASK_HANDLER_MUTEX_PRIORITY_INVERSION); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_MUTEX_PRIORITY_INVERSION_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_MUTEX_PRIORITY_INVERSION,
            "START_TASK_MUTEX_PRIORITY_INVERSION",
            START_STK_SIZE,
            NULL,
            START_TASK_MUTEX_PRIORITY_INVERSION_PRIO,
            &START_TASK_HANDLER_MUTEX_PRIORITY_INVERSION
    );

    vTaskStartScheduler();// 开启任务调度器;
}
