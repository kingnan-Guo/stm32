## 标准库开发 stm32

1、 引入 Liaraies 文件夹
    内部依赖文件
2、 引入 User 文件夹 包含
    main.c
    中断相关
3、  /stm32F103C8T6Project/Libraries/CMSIS/stm32f10x.h 文件中 添加
    #define USE_STDPERIPH_DRIVER
    #define STM32F10X_HD

4、修改  /stm32F103C8T6Project/CMakeLists.txt 文件
    修改

    include_directories(
        Libraries/CMSIS
        Libraries/STM32F10x_StdPeriph_Driver/inc
        User
    )
    
    ## 文件中要包含启动文件
    file(GLOB_RECURSE SOURCES "startup/*.*" "Libraries/*.*" "User/*.*")

    

配置 下载 && Debug 过程中
![GDB.png](readmeFile%2FGDB.png)
    openOCD 面板配置文件
    /Users/kingnan/Documents/stm32Project/stm32F103C8T6Project/stlink.cfg

![OCD.png](readmeFile%2FOCD.png)
    /usr/local/bin/openocd
    -f interface/stlink-v2.cfg -f target/stm32f1x.cfg
   


