#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"

#define vTask2F_uxStackDepth    100
#define vTask2F_uxPriority      1
TaskHandle_t vTask2F_pxCreatedTask; // 存放任务句柄

void vTask1Function(void *pvParameters){
    OLED_ShowString(2, 1, "vTask1Function");
}

void vTask2Function(void *pvParameters){
    OLED_ShowString(4, 1, pvParameters);
}

void xTaskCreateFunMain(void ){

    TaskHandle_t xHandleTask1;
    xTaskCreate(vTask1Function, "Task1", 100, NULL, 1, &xHandleTask1);

    xTaskCreate(
            (TaskFunction_t)                vTask2Function, // 任务函数
            (char *  )                          "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)                      vTask2F_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            (void *  )                      "vTask2",// 传递给任务函数的参数
            (UBaseType_t)                       vTask2F_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                &vTask2F_pxCreatedTask // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
//    xTaskCreate(vTask2Function, "Task2", 100, NULL, 1, &vTask2F_pxCreatedTask);



    vTaskStartScheduler();// 开启任务调度器;
}