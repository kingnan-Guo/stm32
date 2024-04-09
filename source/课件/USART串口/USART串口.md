1、安装 minicom
brew install minicom

2、去dev 看一下
cd /dev ls


3、 使用minicom 配置一下
minicom -s

后来啥也没配置 是因为 硬件的问题

# ========

1、mac 系统下无法判断 \r 只有 \n
2、所以后来用两个 # 包裹数据 然后 判断 \n