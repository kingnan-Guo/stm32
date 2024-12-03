//
// Created by 90175 on 2024/11/26.
//
// 定时器
// 定时器 防抖

//  \/\_
//在 电平 抖动过程中 , 每一次触发 中断 都 复位定时器，然后设置 20ms 作为 定时器时间， 所以 即便发生多次抖动， 最终只会执行一次， 把前面的 中断抖动 消除掉

#include "stm32f10x.h"
#include "OLED.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "Serial.h"
#include "retarget.h"
#include "FREERTOS_TIMER_DEBOUNCE.h"
//#include "countSensor.h"
#include "customizeInterrupt.h"
//任务优先级
#define START_TASK_TIMER_DEBOUNCE_PRIO		     1
//任务堆栈大小
#define START_STK_SIZE 		                     128

#define vTASK1_TIMER_DEBOUNCE_uxStackDepth       128
#define vTASK1_TIMER_DEBOUNCE_uxPriority         3
#define vTASK2_TIMER_DEBOUNCE_uxStackDepth       128
#define vTASK2_TIMER_DEBOUNCE_uxPriority         3

//任务句柄
TaskHandle_t    START_TASK_HANDLER_TIMER_DEBOUNCE;
TaskHandle_t    TASK1_HANDLER_TIMER_DEBOUNCE;
TaskHandle_t    TASK2_HANDLER_TIMER_DEBOUNCE;


// 定时器 任务句柄
static TimerHandle_t xTIMER_DEBOUNCE_HANDLER;

static int FlagTimer = 0;

// 任务1  里 创建定时器 ，并且 启动
void vTASK1_TIMER_DEBOUNCE(void *pvParameters){

    //启动定时器 xTIMER_HANDLER
    xTimerStart(xTIMER_DEBOUNCE_HANDLER, 0);// 启动定时器, 启动定时器 的本质 是把启动定时器的命令发到 定时器命令队列，由守护任务来启动。这个队列又可能满，所以 有可能需要等到 ， 0 不等待
    while (1) {
        printf("vTASK1 running\r\n");
        vTaskDelay(1000);
    }

}

void vTASK2_TIMER_DEBOUNCE(void *pvParameters){
    while (1) {
        printf("vTASK2 running\r\n");
        vTaskDelay(1000);
    }
}


// 定时器 回调
// // 当前项目 由于 每一次进行 按键 操作后 电平抖动会 触发多次 中断，每一次中断 后 复位一次定时器，100ms 后 执行 回调函数
void DEBOUNCETimerCallbackFunction(TimerHandle_t xTimer){
    static  int count = 0;
    FlagTimer = !FlagTimer;
    printf("myTimerCallbackFunction count = %d\r\n", count++);
}

//开始任务任务函数
void START_TASK_TIMER_DEBOUNCE(void *pvParameters)
{

        // 创建定时器
        // 返回值成功 返回 句柄否则返回 NULL
        xTIMER_DEBOUNCE_HANDLER = xTimerCreate(
           "pcTimer",   // 名称
           100,     // 间隔周期 tick 为单位
           pdFALSE,       // pdTRUE 自动加载 pdFALSE 一次性
           NULL,            // 回调函数可使用此为参数，比如分辨是哪个定时器，也可以
           DEBOUNCETimerCallbackFunction  // 回调函数

        );


        xTaskCreate(
                vTASK1_TIMER_DEBOUNCE,
                "Task1",
                vTASK1_TIMER_DEBOUNCE_uxStackDepth,
                "Task1",
                vTASK1_TIMER_DEBOUNCE_uxPriority,
                &TASK1_HANDLER_TIMER_DEBOUNCE
        );
        xTaskCreate(
                vTASK2_TIMER_DEBOUNCE, // 任务函数
                "vTask2",// 函数 名称， 任务名称长度不要超过  configMAX_TASK_NAME_LEN。
                vTASK2_TIMER_DEBOUNCE_uxStackDepth,// 任务堆栈大小 ，注意 ： 实际申请到的堆栈是 uxStackDepth 的 4 倍（ 一个 StackType_t 是 32 位  4 个字节）， 其中空闲任务 的堆栈大小为    configMINIMAL_STACK_SIZE。
                "vTask2",// 传递给任务函数的参数
            vTASK2_TIMER_DEBOUNCE_uxPriority,// 任务优先级 范围 0～ configMAX_PRIORITIES-1
            &TASK2_HANDLER_TIMER_DEBOUNCE // 任务句柄，任务创建成功以后会返回次惹怒我的任务句柄， 这个 句柄其实就是任务的 任务堆栈，此参数 就用来保存这个任务句柄；其他API函数可能会使用到这个 句柄
    );
    vTaskStartScheduler();// 开启任务调度器;
}


// TIM2 中断函数 如要使用 改成 TIM2_IRQHandler
void ____TIM2_IRQHandler(void){
    /// TIM_IT_Update 代表 要看 哪个中断标志位
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        //summationCount();


        // 这里放置 定时器相关的 功能， 定时器消除 抖动
        // 定时器 复位; 本质 是 往 定时器命令队列里 写入命令， 守护任务 会根据 复位的命令进行 复位操作 复位定时器
        // 中断中  不能等待
        xTimerReset(xTIMER_DEBOUNCE_HANDLER, 0);//这里的 时间是  ： 当前时间 + 超时时间（100ms）


        // 清除标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


// 中断中 要使用 函数 要加 ISR ； 如要使用 改成 TIM2_IRQHandler
void _____TIM2_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken =  pdFALSE;//
    /// TIM_IT_Update 代表 要看 哪个中断标志位
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
        //summationCount();


        // 这里放置 定时器相关的 功能， 定时器消除 抖动
        // 定时器 复位; 本质 是 往 定时器命令队列里 写入命令， 守护任务 会根据 复位的命令进行 复位操作 复位定时器

        // 在 xTimerResetFromISR 中，xHigherPriorityTaskWoken参数 传入到函数里 ，
        // xTimerResetFromISR 的逻辑 是如果 遇到 需要调度 那么不会直接调度，而是 要记录，记录的标记 就是 xHigherPriorityTaskWoken
        // xTimerResetFromISR 中一定不能是太长时间的 函数，当然也不会去等待， 如果可以写入那么运行 写入，
        //          如果是队列相关的 写入 操作，当前如果是 队列满了，那么 不会等待，如果没有 满那么直接写入，入伙由 函数在等待 获取队列中的值 （比当前优先级大） 正常来说 是要开启调度器， 去执行获取的函数，
        //          但是  ISR 不会 ，只会记录
        // 所以 xHigherPriorityTaskWoken参数 是标记


        xTimerResetFromISR(xTIMER_DEBOUNCE_HANDLER, &xHigherPriorityTaskWoken);//这里的 时间是  ： 当前时间 + 超时时间（100ms）

        // 开启一个 低优先级的中断 进行任务调度， 不会影响 当前中断
        // 如果 xHigherPriorityTaskWoken 为 true 那么 触发 调度，但是不会立刻 引起调度，这里的源码 是设置一个 中断，一会处理一下， 这个 TIM2的 中断 结束之后 才会 即刻 执行 执行 调度的中断，才会正真的调度
        // 任何的调度不会 打断 中断
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        // 清除标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}



//void EXTI15_10_IRQHandler(void) {
//    BaseType_t xHigherPriorityTaskWoken =  pdFALSE;//
//    //判定  中断 标志位 是否 为 1
//    if(EXTI_GetITStatus(EXTI_Line15) == SET){
//        // 执行 中断 程序 start
//        addCount();
//
//        printf("EXTI15_10_IRQHandler count \r\n");
//        if (xTimerResetFromISR(xTIMER_DEBOUNCE_HANDLER, &xHigherPriorityTaskWoken) != pdPASS) {
//            printf("Timer reset failed in ISR!\r\n");
//        }
//        //执行中断程序 end
//        // 最后 清除中断 标志位， 如果不清除  会一直进入到中断程序 中
//        EXTI_ClearITPendingBit(EXTI_Line15);
//
//    }
//}

void EXTI9_5_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken =  pdFALSE;//
    //判定  中断 标志位 是否 为 1
    if(EXTI_GetITStatus(EXTI_Line5) == SET){
        // 执行 中断 程序 start
        addInterruptCount();

        printf("EXTI15_10_IRQHandler count \r\n");

//        if (xTimerResetFromISR(xTIMER_DEBOUNCE_HANDLER, &xHigherPriorityTaskWoken) != pdPASS) {
//            printf("Timer reset failed in ISR!\r\n");
//        }
        //执行中断程序 end
        // 最后 清除中断 标志位， 如果不清除  会一直进入到中断程序 中
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}





void FREERTOS_TIMER_DEBOUNCE_MAIN(){
    Serial_Init();
    RetargetInit(USART1);
    Interrupt_MAIN();
    START_TASK_TIMER_DEBOUNCE("pvParameters");
}
