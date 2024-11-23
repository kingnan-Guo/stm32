//
// Created by 90175 on 2024/11/23.
//


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "xTaskSuspendResume.h"





int16_t num = 0x00;
int16_t num2 = 0x00;
TaskHandle_t xHandleTask1;
TaskHandle_t vTask2F_pxCreatedTask; // 存放任务句柄
TaskHandle_t del_Handler;


void vTask1Function_SR(void *pvParameters){
//    OLED_ShowString(2, 1, "vTask1F0");
    while (1) {

        printf("printf task1=%d\r\n",  num++);
        vTaskDelay(1000);

    }
}


#define vTask2F_uxStackDepth    128
#define vTask2F_uxPriority      5
void vTask2Function_SR(void *pvParameters){
    while (1) {
        printf("printf test2\r\n");
        OLED_ShowString(4, 1, "task2 0");
        vTaskDelay(1500); // Delay for 500 milliseconds
        OLED_ShowString(4, 1, "task2 1");


    }

}



void SR_delFunction_SR(void *pvParameters){
    while (1) {
        OLED_ShowNum(3, 1, num, 5);
        if(num  ==  5){
            vTaskDelete(del_Handler); //删除开始任务

        }
    }

}



//任务优先级
#define start_task_SR_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		128
//任务句柄
TaskHandle_t StartTask_Handler_SR;
//开始任务任务函数
void start_task_SR(void *pvParameters)
{
//    taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(vTask1Function_SR, "Task1", 128, NULL, 3, &xHandleTask1);
    xTaskCreate(
            (TaskFunction_t)                          vTask2Function_SR, // 任务函数
            (char *  )                                   "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTask2F_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                               "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                               vTask2F_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                        &vTask2F_pxCreatedTask // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    xTaskCreate(SR_delFunction_SR, "delFunction_SR", 128, NULL, 2, &del_Handler);
    vTaskDelete(StartTask_Handler_SR); //删除开始任务;  为什么执行完成要删除？？？
//    taskEXIT_CRITICAL();            //退出临界区
}



void xTaskSuspendResume_MAIN(void ){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(start_task_SR, "start_task_SR", START_STK_SIZE, NULL, start_task_SR_PRIO, &StartTask_Handler_SR);

    vTaskStartScheduler();// 开启任务调度器;
}
