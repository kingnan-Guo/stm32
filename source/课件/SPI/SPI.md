
# 软件 SPI
# 存储器 不能跨页 写入
如果写入的 地址是从 0x0000FF 的那么，在写入 4 个字节 的时候，
第一个数据在 第一页的最后一位 0x0000FF
第二个数据 在 0x000000
第三个数据 在 0x000001
第四个数据 在 0x000001


但是读取是可以跨页的
从 0x0000FF 读取，读取到的地址 是 
0x0000FF    0x000100    0x000101    0x000102

0x0000FF 有值 
其他的都是 FF

如果一定 要跨页写入 那么只能 再次启动一次 SPI


# 硬件 SPI 
硬件 SPI 的选择
```c
SPI1
SPI_SCK  GPIOA  GPIO_Pin_5  // SCK      SPI 时钟
SPI_D0   GPIOA  GPIO_Pin_6  // MISO     SPI主机输入从机输出
SPI_D1   GPIOA  GPIO_Pin_7  // MOSI     SPI主机输出从机输入
SPI_CS   GPIOA  GPIO_Pin_4  // SS       SPI 片选； NSS 没必要一定接再 PA 4 
```
```c
SPI2
SPI_SCK  GPIOB  GPIO_Pin_13  // SCK      SPI 时钟
SPI_D0   GPIOB  GPIO_Pin_14  // MISO     SPI主机输入从机输出
SPI_D1   GPIOB  GPIO_Pin_15  // MOSI     SPI主机输出从机输入
SPI_CS   GPIOB  GPIO_Pin_12  // SS       SPI 片选； NSS 没必要一定接再 PA 4 
```

```c
SPI1 引脚重定义; 要解除 调试引脚的复用 和 重定义 引脚的功能

SPI_SCK  GPIOB  GPIO_Pin_3  // SCK      SPI 时钟
SPI_D0   GPIOB  GPIO_Pin_4  // MISO     SPI主机输入从机输出
SPI_D1   GPIOB  GPIO_Pin_5  // MOSI     SPI主机输出从机输入
SPI_CS   GPIOA  GPIO_Pin_15  // SS       SPI 片选； NSS 没必要一定接再 PA 4 

```

# 清除标志位

1、发送缓冲期 TXE 标志位 位1 时 表明 发送缓冲期 为 空  ，可以写下一个  待发送的数据 ；当写入 SPI_DR 时， TXE 被清空

2、但是 中断标志位 必须手动清除
