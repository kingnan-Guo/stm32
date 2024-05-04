# FreeRTOS

# 1、移植过程

将 官网的 引入进来
```cmake

include_directories(
        #官网版
        FreeRTOS/include
        FreeRTOS/portable/GCC/ARM_CM3
        FreeRTOS/config
)
file(GLOB_RECURSE SOURCES_RTOS  "FreeRTOS/*.*")


# 这里 新增 SOURCES_RTOS
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${SOURCES_RTOS})

```




将cubeMX 生成的 引入进来
```cmake
    include_directories(
            # cubeMX 版
            Middlewares/Third_Party/FreeRTOS/Source/config
            Middlewares/Third_Party/FreeRTOS/Source/include
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
            #Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    )
    file(GLOB_RECURSE SOURCES_RTOS  "Middlewares/*.*")

# 这里 新增 SOURCES_RTOS
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${SOURCES_RTOS})

```



# 2、cmakeList
```cmake

# 使用 cubeMX_original 来判定

# RTOS
set(cubeMX_original "cubeMX_FreeRTOS")
# 使用if语句判定MY_VAR的值
if(cubeMX_original STREQUAL "cubeMX_FreeRTOS")
    message("cubeMX_FreeRTOS")
    # 添加 .h
    include_directories(
            # cubeMX 版
            Middlewares/Third_Party/FreeRTOS/Source/config
            Middlewares/Third_Party/FreeRTOS/Source/include
            Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3
            #Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    )
    file(GLOB_RECURSE SOURCES_RTOS  "Middlewares/*.*")


elseif(cubeMX_original STREQUAL "original_FreeRTOS")
    message("FreeRTOS")
    # 添加 .h
    include_directories(
            #官网版
            FreeRTOS/include
            FreeRTOS/portable/GCC/ARM_CM3
            FreeRTOS/config
    )
    file(GLOB_RECURSE SOURCES_RTOS  "FreeRTOS/*.*")
else()
    message("不使用 FreeRTOS")
    file(GLOB_RECURSE SOURCES_RTOS  "")
endif()



# 这里 新增 SOURCES_RTOS
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${SOURCES_RTOS})

```



# 3、关于  FreeRTOSConfig.h

```markdown

当前 两个 FreeRTOS 的 FreeRTOSConfig.h 是不同的 

FreeRTOS/config/FreeRTOSConfig.h
官网中 添加了
// FreeRTOS 需要这个 要不会报错// FreeRTOS 需要这个 要不会报错； 动静态内存分配相关
#define configKERNEL_PROVIDED_STATIC_MEMORY 1

```


```markdown
cubeMX_freeRTOS 中 就是 从 cubeMX生成后直接 复制过来
Middlewares/Third_Party/FreeRTOS/Source/config/FreeRTOSConfig.h

```





================

b站 给的 案例 
忘记给仿真链接了，两个案例的仿真链接如下，你们可以去试试。
多线程：https://wokwi.com/projects/395652927413886977
消息队列：https://wokwi.com/projects/395653037768128513

====================



# FreeRTOS  任务特性

简单
没有 使用限制
支持抢占
支持优先级
每个任务都有堆栈 导致了RAM 使用量增大； 在A 任务 切换到B 任务， 要把当前CPU 的上下问保存到 堆栈里
如果使用抢占的话必须自卸考虑重如的问题

任务状态
就绪态 ： 已经准备好，下次运行的时候就可以 使用；任务调度器 调取了 就绪态的任务 ，这时 任务达到运行态； B任务正在运行，这时 来个个更高优先级的 A 任务，这时 B 任务 就变成了 就绪态
阻塞态： 某个任务 等待 信号量 或者消息队列； 等待事件发生之后 从 阻塞态出来； 从运行态到 阻塞态 ，调取 阻塞的函数；
挂起态； 暂停运行
![任务状态.png](%E4%BB%BB%E5%8A%A1%E7%8A%B6%E6%80%81.png)

# 任务实现
不能从任务函数中国返回或者退出， 从任务函数中返回或 退出 的会啊会调用 configASSERT() ,前提时 要定义configASSERT，如果一定要从任务函数中 退出的话 那一定要调用 函数的 vTaskDelete(NULL) 来删除此任务


```c
void vTask2(void *pvParameters) {
    for (;;) {
        // Toggle LED2
        OLED_ShowString(4, 1, "FreeRtos 2");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1000 milliseconds
        OLED_ShowString(4, 1, "FreeRtos 3");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1000 milliseconds
    }
    vTaskDelete(NULL)
}


```

# 任务控制快
描述任务属性的数据结构成为 任务控制快 ， 为 TCB_t；  我觉得 就是配置 任务的 结构体

FreeRTOS/tasks.c 文件内的   `typedef struct tskTaskControlBlock {} tskTCB`    `typedef tskTCB TCB_t;`




