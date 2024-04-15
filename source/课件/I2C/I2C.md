i2c

第一步 验证 是否可以 得到 ACK 

接线图如下


![I2C接线图.jpeg](I2C%E6%8E%A5%E7%BA%BF%E5%9B%BE.jpeg)


# 计算
![各个轴的数据.jpeg](%E5%90%84%E4%B8%AA%E8%BD%B4%E7%9A%84%E6%95%B0%E6%8D%AE.jpeg)

```c
左下脚的数据为 加速段z轴的 值； 也就是 重力加速度
    计数公式 如下：

    1848 / 32768 =  X / 16 ; 由于 最大量程 选择了  16g;
    
    X = 0.9023 g ; 约等于 1 g
            
```




# STM32 外设 I2C 连线
SCL_PIN 连 PB6 
SDA_PIN 连 PB7

![I2C外设驱动连线方式.jpeg](I2C%E5%A4%96%E8%AE%BE%E9%A9%B1%E5%8A%A8%E8%BF%9E%E7%BA%BF%E6%96%B9%E5%BC%8F.jpeg)