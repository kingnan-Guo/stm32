//
// Created by kingnan on 2024/12/1.
//
// 信号量
// 计数

// 当前这里 有问题，老师留了作业，晚上尝试


// 调整任务 顺序 可以让 任务2 先执行 进行打印 后 进入到 阻塞 状态 ，然后 执行 任务1 ， 数据准备完毕后， 任务2 再 执行
// 这个项目中 任务3 任务4 不会执行 除非 添加 Delay，
// 任务 1 2 与 任务 3 4 只能分开执行 因为  内存爆了



#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_SEMAPHORE.h"



//任务优先级
#define START_TASK_SEMAPHORE_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                    128

#define vTASK1_SEMAPHORE_FUNCTION_uxStackDepth       640
#define vTASK1_SEMAPHORE_FUNCTION_uxPriority         2
#define vTASK2_SEMAPHORE_FUNCTION_uxStackDepth       640
#define vTASK2_SEMAPHORE_FUNCTION_uxPriority         2


//任务句柄
TaskHandle_t    START_TASK_HANDLER_SEMAPHORE;
TaskHandle_t    TASK1_HANDLER_SEMAPHORE;
TaskHandle_t    TASK2_HANDLER_SEMAPHORE;
TaskHandle_t    TASK3_HANDLER_SEMAPHORE;
TaskHandle_t    TASK4_HANDLER_SEMAPHORE;
TaskHandle_t    DELETE_HANDLER_SEMAPHORE;

static SemaphoreHandle_t xSEMAPHORE_HANDLE_COUNT;//信号量 句柄
static SemaphoreHandle_t xSEMAPHORE_HANDLE_BIN;// 二进制 信号量 句柄

static int sum = 0;
static volatile int flagCalcEnd = 0;
void vTASK1_SEMAPHORE(void *pvParameters){
    vTaskDelay(100);// 这里 只有 在 有延时的时候 才会 先去 执行 任务2  我不太理解的是 不是先执行 任务二吗 为啥 注释掉 这里 就变成 任务二 没有执行； 但实际上，任务1可能长时间运行或其他调度问题影响了这一点。
    volatile int i = 0;
    uint16_t vTASK3_QUEUE_NUM;
    //printf("%s\r\n", (char *)pvParameters);
    OLED_ShowString(1,1, (char *)pvParameters);
    while (1){
        for (i = 0; i < 1000; ++i) {
            sum++;
            OLED_ShowNum(1, 2, sum, 5);
        }
        xSemaphoreGive(xSEMAPHORE_HANDLE_COUNT);// 信号量 的 give 函数
        printf("vTASK1_SEMAPHORE %d\r\n", vTASK3_QUEUE_NUM++);
        OLED_ShowNum(1, 8, i++, 5);
        //sum = 0;
        //vTaskDelay(1);
        //vTaskDelete(NULL);//
    }
}


// 当前这里 有问题，老师留了作业，晚上尝试
void vTASK2_SEMAPHORE(void *pvParameters){
    printf("vTASK2_SEMAPHORE start \r\n");
    //vTaskDelay(1);
    while (1) {
        flagCalcEnd = 0;
        //printf("vTASK2_SEMAPHORE start \r\n");
        // 阻塞 等待 知道 它 出现； 任务 1 释放 xSemaphoreGive ， vTASK2_SEMAPHORE 会被唤醒
        xSemaphoreTake(xSEMAPHORE_HANDLE_COUNT, portMAX_DELAY);//
        printf("vTASK2_SEMAPHORE sum %d\r\n", sum);// 这里获取到值不是  1000 ，具体是什么问题？？？？？、
        flagCalcEnd = 1;
    }
}


// 任务 3  任务 4 共同使用的 函数； 二进制信号量 互斥的  测试函数
void vTASK3_SEMAPHORE(void *pvParameters){
    while (1) {
        // 阻塞 等待 知道 它 出现； 任务 3 和 任务 4 就是 互斥的， 只有一个 可以使用  print 串口
        // 这里是 二进制 信号量
        xSemaphoreTake(xSEMAPHORE_HANDLE_BIN, portMAX_DELAY);
        printf("%s\r\n", (char *)pvParameters);
        xSemaphoreGive(xSEMAPHORE_HANDLE_BIN);
        vTaskDelay(1000);// 没有 vTaskDelay 全是 vtask 3
    }
}



void vDELETE_SEMAPHORE(void *pvParameters){
    while (1) {
    }
}
//
//void vApplicationMallocFailedHook(void) {
//    printf("内存分配失败！\r\n");
//    while (1); // 卡住，方便调试
//}



//开始任务任务函数
void START_TASK_SEMAPHORE(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区

    // 创建 计数型 信号量
    xSEMAPHORE_HANDLE_COUNT = xSemaphoreCreateCounting(10, 0);// 最大值 是 10， 初始值 是 0

    // 创建 二进制 信号量
    xSEMAPHORE_HANDLE_BIN = xSemaphoreCreateBinary();
    xSemaphoreGive(xSEMAPHORE_HANDLE_BIN);// 二进制信号量 默认 创建 是 0 ，所以这里传入 1


//
//    xTaskCreate(
//            (TaskFunction_t)                           vTASK2_SEMAPHORE, // 任务函数
//            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
//            (const configSTACK_DEPTH_TYPE)            vTASK2_SEMAPHORE_FUNCTION_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
//            // (uint32_t)                                         vTask2F_uxStackDepth,
//            (void *  )                                "free2 0",// 传递给任务函数的参数
//            (UBaseType_t)                                vTASK2_SEMAPHORE_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
//            (TaskHandle_t *)                         &TASK2_HANDLER_SEMAPHORE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
//    );
//
//    xTaskCreate(
//            vTASK1_SEMAPHORE,
//            "Task1",
//            vTASK1_SEMAPHORE_FUNCTION_uxStackDepth,
//            NULL,
//            vTASK1_SEMAPHORE_FUNCTION_uxPriority,
//            &TASK1_HANDLER_SEMAPHORE
//    );



    // 任务 3  4 与   任务 1 2  要分开执行
    // 信号量  互斥
    xTaskCreate(
            vTASK3_SEMAPHORE,
            "vTask3",
            640,
            "vTask3 param",
            3,
            &TASK3_HANDLER_SEMAPHORE
    );
    xTaskCreate(
            vTASK3_SEMAPHORE,
            "vTask4",
            640,
            "vTask4 param",
            3,
            &TASK4_HANDLER_SEMAPHORE
    );


    vTaskDelete(START_TASK_HANDLER_SEMAPHORE); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_SEMAPHORE_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_SEMAPHORE,
            "START_TASK_SEMAPHORE",
            START_STK_SIZE,
            NULL,
            START_TASK_SEMAPHORE_PRIO,
            &START_TASK_HANDLER_SEMAPHORE
    );

    vTaskStartScheduler();// 开启任务调度器;
}




//原因分析
//        任务创建后立即准备就绪
//在FreeRTOS中，当一个任务被创建时，如果调度器已经启动，那么根据任务的优先级，调度器会立即切换到优先级最高的任务执行。然而，在你的代码中，任务创建发生在START_TASK_MUTEX_PRIORITY_INVERSION中，这个函数本身是一个任务，优先级为1。
//
//当START_TASK_MUTEX_PRIORITY_INVERSION依次创建任务1、任务2和任务3时，调度器并不会立即切换到新创建的最高优先级任务，而是等待当前任务（即START_TASK_MUTEX_PRIORITY_INVERSION）执行完成并调用vTaskDelete()后才重新调度。
//
//任务就绪和运行的时间窗口
//        创建任务时，任务1（优先级2）、任务2（优先级3）和任务3（优先级4）都进入了“就绪状态”。当START_TASK_MUTEX_PRIORITY_INVERSION删除自身后，调度器会挑选优先级最高的任务运行。然而，此时任务3中调用了vTaskDelay(30ms)，进入了“延迟阻塞”状态，任务2也调用了vTaskDelay(100ms)，只有任务1没有阻塞，因此任务1被调度执行。
//
//任务阻塞和调度器的决定
//        任务3虽然优先级最高，但由于在初始化时调用了vTaskDelay(30ms)，它进入了阻塞态；任务2也类似。因此，在延迟时间未到达之前，任务3和任务2都不会运行，调度器只能选择运行没有阻塞的任务1。
//
//互斥信号量的影响
//        任务1在运行时获取了互斥信号量。根据代码中的逻辑，当任务3尝试获取信号量时，会被阻塞，导致任务3无法执行。这种情况加剧了任务1先执行的现象。