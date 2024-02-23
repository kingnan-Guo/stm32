## 第一个可以在使用 cubeide 可以烧录到  stm32C8T6 开发板上的程序
可以执行  
点亮led PC13 PA11

问题
1、st link 无法连接 但是可以看到序列号


解决：
1、在 https://www.st.com/content/my_st_com/zh/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-performance-and-debuggers/st-link-server.html#get-software  下载
安装后 应该是成功了，但是 开发版 不闪灯

2、打包报错

/include/stdint.h:9:16: fatal error: stdint.h: No such file or directory
9 | # include_next <stdint.h>
|                ^~~~~~~~~~
compilation terminated.

解决：
安装 整个 交叉编译链
brew install --cask gcc-arm-embedded





## =========
/Users/kingnan/.vcpkg-clion/vcpkg



make -j7 all 


## ============
嵌入式 GDB 配置
localhost:3333
/usr/local/bin/openocd
-f interface/stlink-v2.cfg -f target/stm32f1x.cfg

##===
在 debugger 情况下 外设 设置 svd 的路径
添加 svd 在 官网下载  svd 并且放在 /Users/kingnan/Documents/stm32 program/en.stm32f1_svd/STM32F1_svd_V1.2/STM32F103.svd
中





