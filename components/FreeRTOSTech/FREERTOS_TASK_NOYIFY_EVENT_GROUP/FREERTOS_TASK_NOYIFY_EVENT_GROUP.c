//
// Created by 90175 on 2024/11/26.
// 任务 通知  轻量级事件组 的使用

//任务3 先执行 ， 到 xTaskNotifyWait 阻塞
//任务1 执行 将 传入 bit0， 这时 会 触发 任务3 ，但是任务3 中 有判断  if(bits & ((1<<0) | (1<<1))) ，只有 bit0 和 bit1 都 为1 的时候 才继续向下执行


#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TASK_NOYIFY_EVENT_GROUP.h"





//任务优先级
#define START_TASK_TASK_NOYIFY_EVENT_GROUP_PRIO		                1
//任务堆栈大小
#define START_STK_SIZE 		                            128

#define vTASK1_TASK_NOYIFY_EVENT_GROUP_uxStackDepth        512
#define vTASK1_TASK_NOYIFY_EVENT_GROUP_uxPriority          3
#define vTASK2_TASK_NOYIFY_EVENT_GROUP_uxStackDepth        512
#define vTASK2_TASK_NOYIFY_EVENT_GROUP_uxPriority          3
#define vTASK3_TASK_NOYIFY_EVENT_GROUP_uxStackDepth        1024
#define vTASK3_TASK_NOYIFY_EVENT_GROUP_uxPriority          3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_TASK_NOYIFY_EVENT_GROUP;
TaskHandle_t    TASK1_HANDLER_TASK_NOYIFY_EVENT_GROUP;
TaskHandle_t    TASK2_HANDLER_TASK_NOYIFY_EVENT_GROUP;
TaskHandle_t    TASK3_HANDLER_TASK_NOYIFY_EVENT_GROUP;

// 队列 的 句柄
static QueueHandle_t xQueue_Handle;// 创建 队列 的 句柄

// 事件组 句柄
static EventGroupHandle_t EVENT_GROUP_CALC;

static int sum = 0;
static int dec = 0;


void vTASK1_TASK_NOYIFY_EVENT_GROUP(void *pvParameters){
    int i = 0;
    while (1) {
        printf("vTASK1 start\r\n");
        for (i = 0; i < 1000000; ++i) {
            sum++;
        }
        // 队列里插入数据
        xQueueSend(xQueue_Handle, &sum, 0);

        // 设置 事件 0
        // xEventGroupSetBits(EVENT_GROUP_CALC, 1<<0);// EVENT_GROUP_CALC 事件组 的  第 0 位
        xTaskNotify(TASK3_HANDLER_TASK_NOYIFY_EVENT_GROUP, (1<<0), eSetBits);// 使用 eSetBits 通知值 =  原来的值 | ulValue ， 按位或
        
        printf("vTASK1 set bit 0 \r\n");
        vTaskDelete(NULL);
    }
}



void vTASK2_TASK_NOYIFY_EVENT_GROUP(void *pvParameters){
    const TickType_t xDelay5ms = pdMS_TO_TICKS( 5UL );
    while (1) {
        printf("vTASK2 start\r\n");
        for (int i = 1000000; i >0 ; i--) {
            dec--;
        }
        // 队列里插入数据
        xQueueSend(xQueue_Handle, &dec, 0);

        // 设置 事件 1
        //xEventGroupSetBits(EVENT_GROUP_CALC, 1<<1); // EVENT_GROUP_CALC 事件组 的  第 1 位
        xTaskNotify(TASK3_HANDLER_TASK_NOYIFY_EVENT_GROUP, (1<<1), eSetBits);// 通知 任务 3

        printf("vTASK2 set bit 1 \r\n");
        vTaskDelete(NULL);
    }
}


void vTASK3_TASK_NOYIFY_EVENT_GROUP(void *pvParameters){
    const TickType_t xDelay5ms = pdMS_TO_TICKS( 5UL );
    uint32_t val1;
    uint32_t val2;

    uint32_t bits;
    while (1) {
        // 等待 事件
        // xEventGroupWaitBits(
        //         EVENT_GROUP_CALC,
        //         (1<<0) | (1<<1), // 等待哪些事件 ： 0 位  、 1 位
        //         pdTRUE,// 是否清除 事件, pdTRUE 清除
        //         pdTRUE,// 是等待 两个都发生 还是 其中一个
        //         portMAX_DELAY// 等待时间长度, 最大长度
        // );




        xTaskNotifyWait(
                0, // ulBitsToClearOnEntry: 调这个函数 是否要清除 某一位 0 不需要,
                0, // ulBitsToClearOnExit:  退出 的时候 要不要清除 某一位 ， 0 不需要
                &bits, // pulNotificationValue: 获取的值
                portMAX_DELAY// xTicksToWait: 等待多久
        );
        // 判断 bit0 bit1  是不是都发生了
        // if(bits & ((1<<0) | (1<<1))){
        if((bits & 0x30) == 0x30){// 为啥？？
            vTaskDelay(20);
            // 队列里 连续 取出两条数据
            xQueueReceive(xQueue_Handle, &val1, 0);
            xQueueReceive(xQueue_Handle, &val2, 0);
            //printf("%d\r\n", sum);
            printf("val1 = %d ; val2 = %d \r\n", val1, val2);
        } else {

            vTaskDelay(20);
            printf(" error \r\n");

        }

    }
}



//开始任务任务函数
void START_TASK_TASK_NOYIFY_EVENT_GROUP(void *pvParameters)
{

    // 创建 事件组
    EVENT_GROUP_CALC = xEventGroupCreate();
    // 创建 队列
    xQueue_Handle = xQueueCreate(2, sizeof(int));
    if(xQueue_Handle == NULL){
        printf("can not create Queue");
    }

    // 创建三个任务
    xTaskCreate(
            vTASK1_TASK_NOYIFY_EVENT_GROUP,
            "Task1",
            vTASK1_TASK_NOYIFY_EVENT_GROUP_uxStackDepth,
            "vTask1 params",
            vTASK1_TASK_NOYIFY_EVENT_GROUP_uxPriority,
            &TASK1_HANDLER_TASK_NOYIFY_EVENT_GROUP
    );
    xTaskCreate(
            vTASK2_TASK_NOYIFY_EVENT_GROUP, // 任务函数
            "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK2_TASK_NOYIFY_EVENT_GROUP_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask2 params",// 传递给任务函数的参数
            vTASK2_TASK_NOYIFY_EVENT_GROUP_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK2_HANDLER_TASK_NOYIFY_EVENT_GROUP // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    xTaskCreate(
            vTASK3_TASK_NOYIFY_EVENT_GROUP, // 任务函数
            "vTask3",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            vTASK3_TASK_NOYIFY_EVENT_GROUP_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            "vTask3 params",// 传递给任务函数的参数
            vTASK3_TASK_NOYIFY_EVENT_GROUP_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK3_HANDLER_TASK_NOYIFY_EVENT_GROUP // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    vTaskStartScheduler();// 开启任务调度器;

}



void FREERTOS_TASK_NOYIFY_EVENT_GROUP_MAIN(){
    Serial_Init();
    RetargetInit(USART1);

    START_TASK_TASK_NOYIFY_EVENT_GROUP("pvParameters");
}
