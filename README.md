## 标准库开发 stm32
# 输入捕获 测频率 同时 测占空比

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
    ./stlink.cfg

![OCD.png](readmeFile%2FOCD.png)

    /usr/local/bin/openocd
    -f interface/stlink-v2.cfg -f target/stm32f1x.cfg
   


svd 文件加载
 路径 ./stm32_Files/STM32F1_svd_V1.2/STM32F103.svd

![svd文件加载.png](readmeFile%2Fsvd%E6%96%87%E4%BB%B6%E5%8A%A0%E8%BD%BD.png)




## ===

如果 外部输入的信号功率 很小
内部的上拉电阻很小 可能会影响 到这个输入信号
这时候可以用一下浮空输入 防止影响外部输入的电平