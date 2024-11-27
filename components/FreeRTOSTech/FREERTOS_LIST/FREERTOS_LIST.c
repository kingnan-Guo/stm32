//
// Created by 90175 on 2024/11/25.
//

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_LIST.h"
#include "OLED.h"


//任务优先级
#define START_TASK_LIST_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		        128
//任务句柄
TaskHandle_t    START_TASK_HANDLER_LIST;
TaskHandle_t    TASK1_HANDLER_LIST;
TaskHandle_t    TASK2_HANDLER_LIST; // 存放任务句柄
TaskHandle_t    DELETE_HANDLER_LIST;
TaskHandle_t    TASK3_HANDLER_LIST;


// 列表项
List_t TEST_LIST;
ListItem_t TEST_LIST_ITEM1;
ListItem_t TEST_LIST_ITEM2;
ListItem_t TEST_LIST_ITEM3;

void vTASK1_LIST(void *pvParameters){
    int16_t num = 0x00;
    while (1) {
        // printf("printf task1=%d\r\n",  num);
        printf("printf task1=\r\n");
        OLED_ShowString(1, 1, "task1");
        OLED_ShowNum(1, 9, num, 5);
        num++;
        vTaskDelay(1000);
    }
}

void vTASK2_LIST(void *pvParameters){
    int16_t num = 0;
    while (1) {
        printf("printf task2= \r\n");
        // printf("printf data=%d\r\n", data);
        // printf("printf task2=%d\r\n",  num);
        OLED_ShowString(2, 1, "task2");
        OLED_ShowNum(2, 9, num++, 5);
        vTaskDelay(1000); // Delay for 500 milliseconds
    }
}


void vTASK3_LIST(void *pvParameters){
    vListInitialise(&TEST_LIST);// 初始化 列表
    vListInitialiseItem(&TEST_LIST_ITEM1);// 初始化 列表项

    TEST_LIST_ITEM1.xItemValue = 40;
    TEST_LIST_ITEM2.xItemValue = 60;
    TEST_LIST_ITEM3.xItemValue = 50;



    printf("列表 和列表项地址  \r\n");

    printf("项目                  地址        \r\n");
    printf("TEST_LIST             %#x       \r\n", (int)&TEST_LIST);
    printf("TEST_LIST->pxIndex    %#x       \r\n", (int)&TEST_LIST.pxIndex);
    printf("TEST_LIST->xListEnd   %#x       \r\n", (int)&TEST_LIST.xListEnd);
    printf("TEST_LIST_ITEM1       %#x       \r\n", (int)&TEST_LIST_ITEM1);
    printf("TEST_LIST_ITEM2       %#x       \r\n", (int)&TEST_LIST_ITEM2);
    printf("TEST_LIST_ITEM3       %#x       \r\n", (int)&TEST_LIST_ITEM3);

    vListInsert(&TEST_LIST, &TEST_LIST_ITEM1);
    printf(" TEST_LIST_ITEM1  \r\n");
    printf("项目                             地址       \r\n");
    printf("TEST_LIST->xListEnd->pxNext     %#x       \r\n", (int)&TEST_LIST.xListEnd.pxNext);
    printf("TEST_LIST_ITEM1->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM1.pxNext);
    printf("TEST_LIST->xListEnd->pxPrevious %#x       \r\n", (int)&TEST_LIST.xListEnd.pxPrevious);
    printf("TEST_LIST_ITEM1->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM1.pxPrevious);
    printf("TEST_LIST_ITEM2->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM2.pxPrevious);
    printf("TEST_LIST_ITEM3->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM3.pxPrevious);
;




    vListInsert(&TEST_LIST, &TEST_LIST_ITEM2);
    printf(" TEST_LIST_ITEM3  \r\n");
    printf("项目                             地址       \r\n");
    printf("TEST_LIST->xListEnd->pxNext     %#x       \r\n", (int)&TEST_LIST.xListEnd.pxNext);
    printf("TEST_LIST_ITEM1->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM1.pxNext);
    printf("TEST_LIST_ITEM2->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM2.pxNext);

    printf("TEST_LIST->xListEnd->pxPrevious %#x       \r\n", (int)&TEST_LIST.xListEnd.pxPrevious);
    printf("TEST_LIST_ITEM1->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM1.pxPrevious);
    printf("TEST_LIST_ITEM2->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM2.pxPrevious);



    vListInsert(&TEST_LIST, &TEST_LIST_ITEM3);

    printf(" TEST_LIST_ITEM3  \r\n");
    printf("项目                             地址       \r\n");
    printf("TEST_LIST->xListEnd->pxNext     %#x       \r\n", (int)&TEST_LIST.xListEnd.pxNext);
    printf("TEST_LIST_ITEM1->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM1.pxNext);
    printf("TEST_LIST_ITEM2->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM2.pxNext);
    printf("TEST_LIST_ITEM3->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM3.pxNext);
    printf("TEST_LIST->xListEnd->pxPrevious %#x       \r\n", (int)&TEST_LIST.xListEnd.pxPrevious);
    printf("TEST_LIST_ITEM1->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM1.pxPrevious);
    printf("TEST_LIST_ITEM2->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM2.pxPrevious);
    printf("TEST_LIST_ITEM3->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM3.pxPrevious);


    // 移除 2
    uxListRemove(&TEST_LIST_ITEM2);

    printf(" TEST_LIST_ITEM3  \r\n");
    printf("项目                             地址       \r\n");
    printf("TEST_LIST->xListEnd->pxNext     %#x       \r\n", (int)&TEST_LIST.xListEnd.pxNext);
    printf("TEST_LIST_ITEM1->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM1.pxNext);
    printf("TEST_LIST_ITEM3->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM3.pxNext);
    printf("TEST_LIST->xListEnd->pxPrevious %#x       \r\n", (int)&TEST_LIST.xListEnd.pxPrevious);
    printf("TEST_LIST_ITEM1->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM1.pxPrevious);
    printf("TEST_LIST_ITEM3->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM3.pxPrevious);

    // 末尾插入 ，末尾插入 不是从 最后 插入， 二十要 先找打 pxIndex 的 前一个，但是 pxIndex 是可以修改的
    TEST_LIST.pxIndex = TEST_LIST.pxIndex -> pxNext; //修改 pxIndex 位置
    vListInsertEnd(&TEST_LIST, &TEST_LIST_ITEM2);

    printf("vListInsertEnd TEST_LIST_ITEM2  \r\n");
    printf("项目                             地址       \r\n");
    printf("TEST_LIST->xListEnd->pxNext     %#x       \r\n", (int)&TEST_LIST.xListEnd.pxNext);
    printf("TEST_LIST_ITEM1->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM1.pxNext);
    printf("TEST_LIST_ITEM2->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM2.pxNext);
    printf("TEST_LIST_ITEM3->pxNext         %#x       \r\n", (int)&TEST_LIST_ITEM3.pxNext);
    printf("TEST_LIST->xListEnd->pxPrevious %#x       \r\n", (int)&TEST_LIST.xListEnd.pxPrevious);
    printf("TEST_LIST_ITEM1->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM1.pxPrevious);
    printf("TEST_LIST_ITEM2->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM2.pxPrevious);
    printf("TEST_LIST_ITEM3->pxPrevious     %#x       \r\n", (int)&TEST_LIST_ITEM3.pxPrevious);

}

void vDELETE_LIST(void *pvParameters){
    while (1) {
    }
}


//开始任务任务函数
void START_TASK_LIST(void *pvParameters)
{
    //taskENTER_CRITICAL();           //进入临界区
    xTaskCreate(
            vTASK1_LIST,
            "Task1",
            128,
            NULL,
            3,
            &TASK1_HANDLER_LIST
    );



    xTaskCreate(
            (TaskFunction_t)                           vTASK2_LIST, // 任务函数
            (char *  )                                    "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            128,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                NULL,// 传递给任务函数的参数
            (UBaseType_t)                                4,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK2_HANDLER_LIST // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );

    xTaskCreate(
            (TaskFunction_t)                           vTASK3_LIST, // 任务函数
            (char *  )                                    "vTask3",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
            (const configSTACK_DEPTH_TYPE)            128,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
            // (uint32_t)                                         vTask2F_uxStackDepth,
            (void *  )                                "free2 0",// 传递给任务函数的参数
            (UBaseType_t)                                4,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            (TaskHandle_t *)                         &TASK3_HANDLER_LIST // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );



    vTaskDelete(START_TASK_HANDLER_LIST); //删除开始任务;  为什么执行完成要删除？？？
    //taskEXIT_CRITICAL();            //退出临界区
}



void FREERTOS_LIST_INIT_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    xTaskCreate(
            START_TASK_LIST,
            "START_TASK_LIST",
            START_STK_SIZE,
            NULL,
            START_TASK_LIST_PRIO,
            &START_TASK_HANDLER_LIST
    );

    vTaskStartScheduler();// 开启任务调度器;
}
