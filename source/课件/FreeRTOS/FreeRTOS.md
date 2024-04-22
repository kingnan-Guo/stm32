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

