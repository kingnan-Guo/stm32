/* USER CODE BEGIN Header */
/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
/* USER CODE END Header */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */
/* USER CODE END Includes */

//针对不同的编译器调用不同的stdint.h文件
/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;// 只是声明一下系统时钟 ，后面要用到系统时钟
#endif

//在这里 添加此句
//断言 目的是 当 x = 0 时 调用这个函数 输出错误
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int);
//#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__);

/** ==========================================================================================================
 * 基础配置项
 * ==========================================================================================================
 */
#define configUSE_PREEMPTION                     1                          // 1 使用抢占式内核， 高优先级可以抢占低优先级 0 使用协程 （不再更新）
#define configSUPPORT_STATIC_ALLOCATION          1                          // 创建 静态任务 的 宏; 使能 静态方法
//#define configSUPPORT_DYNAMIC_ALLOCATION         1
//#define configUSE_IDLE_HOOK                      0
//#define configUSE_TICK_HOOK                      0
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )        // CPU频率 ； 告诉 FreeRTOS 当前MCU 的主频时多少； 在库函数中 Libraries/CMSIS/system_stm32f10x.c 下定义的 72MHZ ； 配置系统时钟的时候可被更改
#define configTICK_RATE_HZ                       ((TickType_t)1000)         // FreeRTOS 这个系统的节拍频率；  时钟节拍频率，这里设置为1000， 一秒跳动1000次，周期就是1ms； 在 FreeRTOS/portable/GCC/ARM_CM3/portmacro.h 文件中 定义 TickType_t ； uint16_t
#define configMAX_PRIORITIES                     ( 32 )                     // 可使用的最大优先级 ； 数字越大 执行的优先级 越高 ；理论上来说支持的优先级时不限制的 ；之前设置 56 ；只有当 configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 时 可以设置 32 以上的数；硬件指令最大支持 32 位；
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)            // 最小堆栈大小；空闲任务使用的堆栈大小；（堆栈保存运行任务的运行环境，每一个任务都有一个堆栈，堆栈的大小）； 最小堆栈大小只有两个地方 用到，创建空闲任务的时候 FreeRTOS/tasks.c
//#define configTOTAL_HEAP_SIZE                    ((size_t)3072)
#define configMAX_TASK_NAME_LEN                  ( 16 )                     // 任务名字字符串长度 16 个字符
//#define configUSE_TRACE_FACILITY                 1
#define configUSE_16_BIT_TICKS                   0                          // 系统节拍计数器变量数据类型； 1表示为16位无符号整形，0表示为32位无符号整形；（用来表示变量类型 在FreeRTOS 中 configUSE_16_BIT_TICKS 为 1 会定义 configTICK_TYPE_WIDTH_IN_BITS 的类型 为 TICK_TYPE_WIDTH_32_BITS， configTICK_TYPE_WIDTH_IN_BITS 会 定义 TickType_t 的类型， 也就是 间接 定义了 系统节拍计数器变量数据类型， 当前使用stm32 所以时32 位的，所以 TickType_t 是 uint32_t ）（FreeRTOS/portable/GCC/ARM_CM3/portmacro.h）
#define configUSE_MUTEXES                        1                          // 为 1 时 使用互斥信号量
#define configQUEUE_REGISTRY_SIZE                8                          // 不为 0 时表示启用队列记录，具体的值是可以 记录的队列和 信号量最大 数目； 队列注册的大小； （跟调试相关）
#define configUSE_RECURSIVE_MUTEXES              1                          // 1 递归互斥信号量 （具体啥用法 之后再 理解）
#define configUSE_COUNTING_SEMAPHORES            1                          //  1 计数型信号量； 使用计数信号量
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1                          // 1 启用特殊方法来选择下一个要运行的任务； 一般是硬件计算前导零指令，如果所使用的MCU没有这些硬件指令的话此宏应该设置为 0 使用软件区计算，但是有个前提是，当 configUSE_PORT_OPTIMISED_TASK_SELECTION  置 1 时； configMAX_PRIORITIES 只能小于 32； STM32 支持 硬件计算 所以可以配置成 1；

/* Co-routine definitions. FreeRTOS与协程有关的配置选项 （不使用协程） */
//#define configUSE_CO_ROUTINES                    0
//#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* Software timer definitions. FreeRTOS与软件定时器有关的配置选项 */
//#define configUSE_TIMERS                         1
//#define configTIMER_TASK_PRIORITY                ( 2 )
//#define configTIMER_QUEUE_LENGTH                 10
//#define configTIMER_TASK_STACK_DEPTH             256

/**
 *  在 课程中新加的一些 配置
 *
 */
#define configUSE_TIME_SLICING					 1						    // 1 使能时间片调度(默认式使能的) （同优先级的 任务 使用这个 时间片 调度）
#define configUSE_TICKLESS_IDLE					 0                          // 1 启用 低功耗 tickless 模式  ； 自带的 tickless 模式
#define configUSE_QUEUE_SETS					 1                          // 1 为1时启用队列集
#define configIDLE_SHOULD_YIELD					 1                          // 1 为1时空闲任务放弃CPU使用权给其他同优先级的用户任务（其他任务）； FreeRTOS 一个优先级下由多个任务； 空闲任务也是一样，所以 FreeRTOS 最少也会有一个任务，叫做 空闲任务； 在CPU没有其他任务要执行的时候，就去执行这个空闲任务； 所以这里可以配置 把应用任务 的 优先级 排到空闲 任务之前（空闲的时候 给CPU 找点事 做，没有任何特殊的需求）
#define configUSE_TASK_NOTIFICATIONS             1                          // 1 为1时开启任务通知功能，默认开启
#define configCHECK_FOR_STACK_OVERFLOW			 0                          // 0 大于0时启用堆栈溢出检测功能，如果使用此功能用户必须提供一个栈移出钩子函数每如果使用的话，辞职可以位 1或者 2 ，因为由两种栈溢出检测方法（一般调试阶段会使用）
#define configUSE_MALLOC_FAILED_HOOK			 0                          // 1 使用内存申请失败钩子函数； 具体函数需要用户 自己 定义，当前未启用
#define configUSE_APPLICATION_TASK_TAG			 0                          // 1 用户任务标签 （未启动）


/** ==========================================================================================================
 *  FreeRTOS与内存申请有关配置选项 （）把之前上面写的 配置移下来
 * ==========================================================================================================
 */
#define configSUPPORT_DYNAMIC_ALLOCATION         1                          // 支持动态内存申请 ； 动态内存申请就是 标准C库 的Malloc 和 Free 一样； 再FreeRTOS 实现动态内存管理； heap4 内存池 大小 默认 ((size_t)46 * 1024)
#define configTOTAL_HEAP_SIZE                    ((size_t)3072)             // 系统所有总的堆大小；内存池大小


/** ==========================================================================================================
 * FreeRTOS与钩子函数有关的配置选项
 *  ==========================================================================================================
 */
#define configUSE_IDLE_HOOK                      0                          //1，使用空闲钩子；0，不使用 ； 空闲任务 钩子函数
#define configUSE_TICK_HOOK                      0                          //1，使用时间片钩子；0，不使用； 时钟结算 钩子函数 ？？？

/** ==========================================================================================================
 * FreeRTOS与运行时间和任务状态收集有关的配置选项
 *  ==========================================================================================================
 */
#define configGENERATE_RUN_TIME_STATS	        0                       //为1时启用运行时间统计功能 ； 可以查看任务运行多久；（新增  未启用）
#define configUSE_TRACE_FACILITY				1                       //为1启用可视化跟踪调试
#define configUSE_STATS_FORMATTING_FUNCTIONS	1                       //与宏 configUSE_TRACE_FACILITY 同时为1时会编译下面3个函数 prvWriteNameToBuffer(),vTaskList(), vTaskGetRunTimeStats() ；  （新增  启用）



/** ==========================================================================================================
 * FreeRTOS与协程有关的配置选项 （不使用协程）
 *  ==========================================================================================================
 */
#define configUSE_CO_ROUTINES 			        0                       //为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )                   //协程的有效优先级数目



/** ==========================================================================================================
 * FreeRTOS与软件定时器有关的配置选项 （在硬件定时器 上 做一些 补充）
 *  ==========================================================================================================
 */
#define configUSE_TIMERS                         1                         // 1 为 1 时 启动软件定时器； 是由 任务来管理的 ，所以要配置 任务优先级
#define configTIMER_TASK_PRIORITY                ( 2 )                     // 软件定时器 优先级 （在FreeRTOS 中 数字越到 优先级 越高，所以 2 算低优先级）
#define configTIMER_QUEUE_LENGTH                 10                        // 软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH             256                       // 软件定时器 任务 堆栈大小 ； 因为有任务 所以必须设置堆栈的大小

/** ==========================================================================================================
 * 可选函数 配置选项
 * ==========================================================================================================
 */
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1       // 设置优先级
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1
#define INCLUDE_xTimerPendFunctionCall      1
#define INCLUDE_xQueueGetMutexHolder        1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_eTaskGetState               1

/*
 * The CMSIS-RTOS V2 FreeRTOS wrapper is dependent on the heap implementation used
 * by the application thus the correct define need to be enabled below
 */
#define USE_FreeRTOS_HEAP_4     // 这里配置是 使用 heap_4

/** ==========================================================================================================
 * 中断相关的配置选项
 * ==========================================================================================================
 */
/* Cortex-M specific definitions. */        //
#ifdef __NVIC_PRIO_BITS
/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
#define configPRIO_BITS         4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15                        //「中断相关」 中断最低优先级

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5                      //「中断相关」 系统 管理的最高优先级

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )     // 「中断相关」
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY  	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )       // 「中断相关」

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* USER CODE BEGIN 1 */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}
/* USER CODE END 1 */

/** ==========================================================================================================
 * 中断服务函数 相关的配置选项
 * ==========================================================================================================
 */
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler                  // 给中断服务 函数 重新命名
#define xPortPendSVHandler PendSV_Handler               // 中断服务函数   ；给中断服务 函数 重新命名；

/* IMPORTANT: This define is commented when used with STM32Cube firmware, when the timebase source is SysTick,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */

#define xPortSysTickHandler SysTick_Handler

/* USER CODE BEGIN Defines */
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */

// FreeRTOS 需要这个 要不会报错// FreeRTOS 需要这个 要不会报错； 动静态内存分配相关
#define configKERNEL_PROVIDED_STATIC_MEMORY 1



/*** ==========================================================================================================
 * 20241126 添加的额 列表 和列表项 相关的  宏定义
 * ==========================================================================================================
 */
#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES   0;// 列表 完整性


#endif /* FREERTOS_CONFIG_H */
