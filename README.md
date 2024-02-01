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


