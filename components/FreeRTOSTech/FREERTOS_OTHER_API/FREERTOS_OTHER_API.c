//
// Created by 90175 on 2024/11/26.
//

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_OTHER_API.h"



//任务优先级
#define START_TASK_FREERTOS_OTHER_API_PRIO		            1
//任务堆栈大小
#define START_STK_SIZE 		                                 128

#define vTASK1_FREERTOS_OTHER_API_FUNCTION__uxStackDepth      256
#define vTASK1_FREERTOS_OTHER_API_FUNCTION_uxPriority         3
#define vTASK2_FREERTOS_OTHER_API_FUNCTION__uxStackDepth      256
#define vTASK2_FREERTOS_OTHER_API_FUNCTION_uxPriority         3
#define vTASK3_FREERTOS_OTHER_API_FUNCTION__uxStackDepth      256
#define vTASK3_FREERTOS_OTHER_API_FUNCTION_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_FREERTOS_OTHER_API;
TaskHandle_t    TASK1_HANDLER_FREERTOS_OTHER_API;
TaskHandle_t    TASK2_HANDLER_FREERTOS_OTHER_API; // 存放任务句柄
TaskHandle_t    TASK3_HANDLER_FREERTOS_OTHER_API; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_FREERTOS_OTHER_API;


void vTASK1_FREERTOS_OTHER_API(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        // printf(" vTASK1_FREERTOS_OTHER_API =%d\r\n",  num);
        printf(" vTASK2_FREERTOS_OTHER_API \r\n");
        OLED_ShowString(1, 1, "task1 = ");
        OLED_ShowNum(1, 8, num, 5);
        num++;
        vTaskDelay(1000);
    }
}



void vTASK2_FREERTOS_OTHER_API(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        // printf(" vTASK1_FREERTOS_OTHER_API =%d\r\n",  num);
        printf("vTASK2_FREERTOS_OTHER_API \r\n");
        OLED_ShowString(2, 1, "task2 = ");
        OLED_ShowNum(2, 8 , num++, 5);
        vTaskDelay(1000); // Delay for 500 milliseconds
    }
}


char InfoBuffer[1000];// 全局变量  任务缓冲区
void vTASK3_FREERTOS_OTHER_API(void *pvParameters){
    int16_t num = 0x00;

    // 获取 修改 任务 优先级  ==============
    UBaseType_t Priority;
    Priority = uxTaskPriorityGet(TASK1_HANDLER_FREERTOS_OTHER_API);// 根据 任务 句柄 获取 优先级
    printf("TASK Priority = %d\r\n", Priority);
    // 修改优先级
    vTaskPrioritySet(TASK2_HANDLER_FREERTOS_OTHER_API, 4);

    // 获取系统所有的 任务状态 ===================
    uint32_t TotalRunTime;
    UBaseType_t taskStatus_array_size;// 任务 大小
    TaskStatus_t * taskStatusArray;// 储存所有的 任务 的 数组
    UBaseType_t taskNumber = uxTaskGetNumberOfTasks();// 获取系统任务 个数
    taskStatus_array_size = taskNumber;
    taskStatusArray = pvPortMalloc(taskStatus_array_size * sizeof(TaskStatus_t));// 创建 内存；FreeRTOS 自带的内存分配
    if(taskStatusArray != NULL){
        taskStatus_array_size = uxTaskGetSystemState(
                taskStatusArray,
                taskStatus_array_size,
                &TotalRunTime
        );

        for (int i = 0; i < taskStatus_array_size; ++i) {
            printf(
                    "%s\t\t%f\t\t\t%d\t\t\t\r\n",
                    taskStatusArray[i].pcTaskName,// 名字
                    (int)taskStatusArray[i].uxCurrentPriority,// 优先级
                    (int)taskStatusArray[i].xTaskNumber// 编号
           );
        }
        // 创建 开始任务 、 IDLE（空闲任务）、Tmr SVC (定时器服务任务)、vTask1， vTask2
    }



    // vTaskGetInfo 获取任务的 状态信息 ===========
//    vTaskGetInfo(
//            (TaskHandle_t) TASK1_HANDLER_FREERTOS_OTHER_API,
//            (TaskStatus_t *) pxTaskStatus,
//            (BaseType_t) xGetFreeStackSpace,
//            (eTaskState) eState
//    );


    // 根据任务名字 获取 任务 句柄; 人居句柄就是任务控制块
    TaskHandle_t  current_task_handle = xTaskGetCurrentTaskHandle();
    TaskHandle_t  name_to_handle = xTaskGetHandle("vTask2");// 获取名称为 vTask2 的任务 句柄


    UBaseType_t SHWM_SIZE = uxTaskGetStackHighWaterMark(name_to_handle);// 用来 获取某个任务的 堆栈 使用 时 最多占用了多少的情况
    printf("SHWM_SIZE 获历史取任务 堆栈 最小还剩多少 字节 %d\r\n", SHWM_SIZE * 4);

    // 获取 任务状态
    eTaskState get_task_state_form_handle= eTaskGetState(name_to_handle);

    printf("task_state  = %d\r\n", get_task_state_form_handle);// 0 时 运行状态

    // 获取 任务调度器 从启动到现在的计数器 xTaskGetTickCount； 在中断中使用 xTaskGenericNotifyFromISR
    //xTaskGetTickCount();
    //xTaskGenericNotifyFromISR()


    //

    xTaskGetSchedulerState();// 获取任务调度器的运行状态



    // 获取当前所有任务信息 ,列一个表格

     vTaskList(InfoBuffer);// 传参是  任务缓冲区
    printf("%s\r\n", InfoBuffer);// 字符串


    // 获取任务运行事件 百分比
    // vTaskGetRunTimeStats();

    //设置任务标签
    //vTaskSetApplicationTaskTag()


    // 设置 线程本地存储 的指针的值
    //SetThreadLocalStoragePointer();

    while (1) {
        printf("vTASK3_FREERTOS_OTHER_API \r\n");
        OLED_ShowString(3, 1, "task3 = ");
        OLED_ShowNum(3, 8 , num++, 5);
        vTaskDelay(1500); // Delay for 500 milliseconds
    }
}


void vDELETE_FREERTOS_OTHER_API(void *pvParameters){
    while (1) {
    }
}





//开始任务任务函数
void START_TASK_FREERTOS_OTHER_API(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_FREERTOS_OTHER_API,
            "Task1",
            vTASK1_FREERTOS_OTHER_API_FUNCTION__uxStackDepth,
            NULL,
            vTASK1_FREERTOS_OTHER_API_FUNCTION_uxPriority,
            &TASK1_HANDLER_FREERTOS_OTHER_API
    );
    xTaskCreate(
            (TaskFunction_t)                           vTASK2_FREERTOS_OTHER_API, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            vTASK2_FREERTOS_OTHER_API_FUNCTION__uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                vTASK2_FREERTOS_OTHER_API_FUNCTION_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_FREERTOS_OTHER_API // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_FREERTOS_OTHER_API,
            "Task3",
            vTASK3_FREERTOS_OTHER_API_FUNCTION__uxStackDepth,
            NULL,
            vTASK3_FREERTOS_OTHER_API_FUNCTION_uxPriority,
            &TASK3_HANDLER_FREERTOS_OTHER_API
    );
    vTaskDelete(START_TASK_HANDLER_FREERTOS_OTHER_API); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_FREERTOS_OTHER_API_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_FREERTOS_OTHER_API,
            "START_TASK_FREERTOS_OTHER_API",
            START_STK_SIZE,
            NULL,
            START_TASK_FREERTOS_OTHER_API_PRIO,
            &START_TASK_HANDLER_FREERTOS_OTHER_API
    );

    vTaskStartScheduler();// 开启任务调度器;
}
