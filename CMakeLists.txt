#此文件从模板自动生成! 请勿更改!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.27)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(stm32F103C8T6Project C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# ----- start
# 这段可以 正常打包而不会报错， 但是在这个项目中  syscalls.c 内将报错的 函数 在其内部创建了一遍
# 添加这段 之后 可以 使用引入的 "stdarg.h"
#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
#CMAKE_EXE_LINKER_FLAGS 用于创建可执行文件的链接器标志； 链接器在创建可执行文件时将使用这些标志
#set(CMAKE_EXE_LINKER_FLAGS " -specs=rdimon.specs -lgcc -lc -lm -lrdimon")
# ---------- end


#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()


include_directories(
        Libraries/CMSIS
        Libraries/STM32F10x_StdPeriph_Driver/inc
        User
        components/delay
        components/LED
        components/key
        components/oled
        components/countSensor
        components/Timer
        components/PWM
        components/servo
        components/DCMotor
        components/IC
        components/Encode
        components/AD
        components/customDMA
        components/ADC_DMA
        components/Serial # 与 /Users/kingnan/Documents/github/stm32/components/Serial/ 相同
        components/Serial_package
        #软件 I2C 通信
        components/customI2C
        components/MPU6050
        #软件 SPI 通信
        components/coustomSPI
        components/W25Q64
        # 重定向 相关
        system/retarget
        system/syscalls
        # FreeRTSO 使用 组建
        components/FreeRTOSDemo
        # 开始学习 rtos
        components/FreeRTOSTech/xTaskCreateFun
        components/FreeRTOSTech/xTaskCreateFun2


        restarComponets/LED_R
        restarComponets/KEY_R
        restarComponets/customizeInterrupt
        restarComponets/Timer_R
        restarComponets/EXTI_Interrupt
        restarComponets/PWM_R
        restarComponets/INPUTCAPTURE_R
        restarComponets/ENCODE_R
        restarComponets/ADC_R
        restarComponets/ADC_MULTI_CHANNEL
        restarComponets/DMA_R
        restarComponets/USART_R
        restarComponets/SOFTWARE_CONTROL_MPU6050
        restarComponets/SOFTWARE_I2C
        restarComponets/HARDWARE_I2C
        restarComponets/HARDWARE_CONTROL_MPU6050
        restarComponets/SOFTWARE_SPI
        restarComponets/SOFTWARE_CONTROL_W25Q64
        restarComponets/HARDWARE_CONTROL_W25Q64
        restarComponets/HARDWARE_SPI


        components/FreeRTOSTech/xTaskCreateStaticFun
        components/FreeRTOSTech/xTaskSuspendResume
        components/FreeRTOSTech/FreeRTOS_NVIC
        components/FreeRTOSTech/FREERTOS_LIST
        components/FreeRTOSTech/FREERTOS_TEMP
        components/FreeRTOSTech/FREERTOS_OTHER_API


        restarComponets/DATA_REGISTER_EXPLORE # 探索 寄存器
        restarComponets/CLASS_EXPLORE # 探索在 项目使用 C++

)

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB)

#file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*")
#官网版
file(GLOB_RECURSE SOURCES "startup/*.*" "Libraries/*.*" "User/*.*" "components/*.*" "system/*.*" "restarComponets/*.*")


# RTOS
set(cubeMX_original "original_FreeRTOS")
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

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})
# 这里 新增 SOURCES_RTOS
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} ${SOURCES_RTOS})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
