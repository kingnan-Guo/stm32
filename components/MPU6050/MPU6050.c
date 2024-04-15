#include "stm32f10x.h"
#include "delay.h"
#include "customI2C.h"
#include "MPU6050_Reg.h"


#define MPU6050_ADDRESS     0xD0
// 使用 宏定 将 寄存器的地址 使用  字符串来表示


// 端口 换名字 宏定义
//#define SCL_PORT    GPIOC
//#define SCL_PIN     GPIO_Pin_15
//#define SDA_PIN     GPIO_Pin_14


/**
 * 超时 退出
 *
 */
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT){
    uint32_t TimeOut;
    TimeOut =1000;
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS){
        TimeOut--;
        if(TimeOut ==0){
            break;
        }
    };
}



/**
 * 指定地址写
 * 参数:
 *  RegAddress 参数 时 8 位 寄存器地址
 *  Data 数据
 *
 *  过程
 *  1、 customI2C_Start
 *  2、发送  1101 0000 写入操作
 *  3、应答位
 *  4、继续发送下一个字节， 指定寄存器地址；进行写入操作
 *  5、应答位
 *  6、要写如 寄存器 中的数据
 *  7、应答位
 *  8、
 */
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data){
    //uint8_t ACK;
    //customI2C_Start();
    //customI2C_SendByte(MPU6050_ADDRESS);// 1101 0000 写入操作
    //// 接收应答
    //ACK = customI2C_ReceiveAckByte();
    //// 继续发送下一个字节， 寄存器的地址； 进行写入操作
    //customI2C_SendByte(RegAddress);
    //ACK = customI2C_ReceiveAckByte();
    //// 要写如 寄存器 中的数据
    //customI2C_SendByte(Data);
    //ACK = customI2C_ReceiveAckByte();
    //// 停止
    //customI2C_Stop();
    //=========


    //生成起始函数 ； 非阻塞性函数， 执行完成后就结束，不管 标志位 是否改变；也不管是否执行到位
    I2C_GenerateSTART(I2C2, ENABLE);
    // 参数2 指定要检查 哪个事件 ;这里是 EV5： I2C_EVENT_MASTER_MODE_SELECT    ； // 主机模式选择； 因为 STM32 默认为从机，发送起始条件后变为主机
    //while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
    // 有超时等待的  EV5
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    //发送从机地址    地上额地址为 方向 发送； I2C_Direction_Transmitter；发送数据都自地带 接收应答的过程
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS,I2C_Direction_Transmitter);
    //发送地址后 这里会产生 EV6 事件
    // I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED 发送数据已选择
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
    // EV8_1事件： TXE=1 移位寄存器空 数据寄存器空 ，写DR寄存器； 但是没有 EV8_1；所以直接希尔DR 发送数据
    I2C_SendData(I2C2,RegAddress);
    // 这时写入了DR ；DR 的Data 转移到移位 寄存器 发送数据，此时波形产生
    // 写入数据后 检查 EV8 事件
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
    // 因为DATA 的最后一个字节发送完就要终止 了； 如果有 连续的发送时 ，需要等待EV8 事件； 但是要发送完最后一个字节，要等待的是 EV8_2 事件
    // EV8_2 ： BTF = 1 移位寄存器完成移位；请求设置停止位， TXE 和 BTF 位由硬件产生停止条件时 清除

    //EV8_2 ： I2C_EVENT_MASTER_BYTE_TRANSMITTED
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);

    // 终止 时序
    I2C_GenerateSTOP(I2C2, ENABLE);
}

/**
* 指定地址读
 * 先写入 ，并制定寄存器 地址
 * 然后开始重新 开始 ，读取数据
 * 如果接收多个 字节，那么使用  for 循环
 *
 *
*/
uint8_t MPU6050_ReadReg(uint8_t RegAddress){
    //uint8_t ACK;
    //uint8_t Data;
    //customI2C_Start();
    //customI2C_SendByte(MPU6050_ADDRESS);// 1101 0000 写入操作
    //// 接收应答
    //ACK = customI2C_ReceiveAckByte();
    //// 继续发送下一个字节， 寄存器的地址； 进行写入操作
    //customI2C_SendByte(RegAddress);
    //ACK = customI2C_ReceiveAckByte();

    //// ========== 开始 接收数据 ========

    //customI2C_Start();
    //// 寄存器的地址； 进行 读取 入操作; 1101 0001
    //customI2C_SendByte((MPU6050_ADDRESS|0x01));
    //ACK = customI2C_ReceiveAckByte();
    //// 接收应答 之后 总线控制权 就正式交给 从机
    //// 主机 开始接收 数据
    //Data =  customI2C_ReceiveByte();
    //customI2C_SendACK(1);// 主机给 应答 1 ，也就是 释放电平，从机没有收到 低电平 ，所以不会再继续 发送数据
    //customI2C_Stop();

    //return Data;


    //======== 硬件



    //生成起始函数 ； 非阻塞性函数， 执行完成后就结束，不管 标志位 是否改变；也不管是否执行到位
    I2C_GenerateSTART(I2C2, ENABLE);
    // 参数2 指定要检查 哪个事件 ;这里是 EV5： I2C_EVENT_MASTER_MODE_SELECT    ； // 主机模式选择； 因为 STM32 默认为从机，发送起始条件后变为主机
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
    //发送从机地址    定义地址为 方向 发送； I2C_Direction_Transmitter；发送数据都自地带 接收应答的过程
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS,I2C_Direction_Transmitter);
    //发送地址后 这里会产生 EV6 事件
    // I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED 发送数据已选择
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
    // EV8_1事件： TXE=1 移位寄存器空 数据寄存器空 ，写DR寄存器； 但是没有 EV8_1；所以直接希尔DR 发送数据
    I2C_SendData(I2C2,RegAddress);
    // 这时写入了DR ；DR 的Data 转移到移位 寄存器 发送数据，此时波形产生
    // 写入数据后 检查 EV8_2 事件 ; I2C_EVENT_MASTER_BYTE_TRANSMITTED  数据发送结束
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
    //指定地址 结束


    //  如果 还有数据在发送的 时候，生成起始位 的时机会延后，发送完毕后再 生成
    // 再 生成起始
    I2C_GenerateSTART(I2C2, ENABLE);
    //发送地址位； 使用 I2C_Direction_Receiver  函数自动将 MPU6050_ADDRESS 的最后一位 置 1；不用手动置1
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS,I2C_Direction_Receiver);
    //寻址后  等待EV6 ； 主机接收的  EV6 ： I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
    //开始接收从机发送的波形
    //在 接收到第一个字节  会有EV6_1 标志位；适合接收 一个字节的情况； 这时 要把ACK置0 ；同时把停止条件生成位 STOP 置1
    //数据都未接收到，规定就是 就是 要在接收到最后一个 字节之前， 就要提前把 ACK 置 0、 设置停止位 STOP；
    // 停止位 也不会 截停 数据接收，会等到 接收完成后 再产生终止条件的波形

    // 如果接收多个字节， 会再EV7 那里 把 ACK 置 0、 设置停止位 STOP；
    // ***** 停止位 设置晚了 就会多一个 时序出来 *****

    I2C_AcknowledgeConfig(I2C2, ENABLE);

    // 配置 停止
    I2C_GenerateSTOP(I2C2, ENABLE);

    //这时等待 EV7 ： RxNE = 1 读取DR寄存器清除 该事件； 接收到一个字节后会产生
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);

    // 读取数据 返回值就是 DR 的数据
    //I2C_ReceiveData(I2C2);
    uint8_t Data = I2C_ReceiveData(I2C2);

    // 最后要把 ACK 置 回  1； 因为再接收最后一个字节之前 临时 置 0； 接收完成之后 要恢复位1 ； 方便接收多个字节

}



/**
 * 初始化 6050
 *
 * 1、配置电源管理寄存器 1
 *  地址 MPU6050_PWR_MGMT_1  ；
 *      内容： 6B 107   PWR_MGMT_1  R/W ： DEVICE_RESET（设备复位）、 SLEEP（解除睡眠）、 CYCLE（循环模式）、 - 、TEMP_DIS（温度失能）、 CLKSEL[2:0]（时钟）
 *                                          0       0          0        0        0       001
 *                                          不复位   解除睡眠    不循环    无关位    不失能    陀螺仪时钟
 *                                          0       0          0        0        0       000
 *                                          不复位   解除睡眠    不循环    无关位    不失能    内部时钟
 *
 *      写入 ： 0x01
 *
 * 2、 配置电源管理寄存器 2
 *      地址： MPU6050_PWR_MGMT_2
 *      内容：6C 108 PWR_MGMT_2 R/W ：
 *          LP_WAKE_CTRL[1:0]（前两位 循环模式 唤醒频率）、STBY_XA、 STBY_YA、 STBY_ZA、 STBY_XG、 STBY_YG、 STBY_ZG；（后6位 每一个轴的待机位）
 *          00                                        0         0         0         0           0       0
 *          不需要                                     不需要待机
 *      写入 ： 0x00
 *
 *
 * 3、 采样 预分频， 八位 决定数据输出的快慢，值越小 越快
 *     地址：MPU6050_SMPLRT_DIV
 *     内容: 19 25 SMPLRT_DIV R/W：
 *          SMPLRT_DIV[7:0]
 *
 *     写入 ：0x09 ； 十分频
 *
 * 4、 配置寄存器
 *  地址： MPU6050_CONFIG
 *  内容：1A 26 CONFIG R/W：
 *          -、 -、 EXT_SYNC_SET[2:0] （外部同步）、 DLPF_CFG[2:0] （数字低通滤波器）
 *          0   0   000                           110
 *                  不需要 外部同步                 最平滑的滤波
 *  写入 ：0x06 ；00 000 110；
 *
 *5、 陀螺仪寄存器
 *  地址：MPU6050_GYRO_CONFIG
 *  内容：
 *      1B 27 GYRO_CONFIG R/W
 *          - - - （前三位自测使能）、 FS_SEL [1:0]（满量选择）、 - - -
 *          0 0 0                   11                      0 0 0
 *          不自测                   最大量程（16 g）          无关位
 *  写入： 0x18 ； 000 11 000
 *
 * 6、加速度计配置寄存器
 *  地址：MPU6050_ACCEL_CONFIG
 *  内容：
 *      1C 28 ACCEL_CONFIG R/W
 *          XA_ST YA_ST ZA_ST（前三位自测使能）、 AFS_SEL[1:0]]（满量选择）、 ACCEL_HPF[2:0]（高通滤波器）
 *          0 0 0                               11                      0 0 0
 *          不自测                               最大量程  （16 g）        用不到所以 000
 *  写入： 0x18 ； 000 11 000
 *
 *
 *
 * 配置完成 之后 陀螺仪内部 就在 连续不断地 进行数据转换了
 *
 * 输出的数据 放在 这些 寄存器里
 * #define	MPU6050_ACCEL_XOUT_H	0x3B
 * #define	MPU6050_ACCEL_XOUT_L	0x3C
 * #define	MPU6050_ACCEL_YOUT_H	0x3D
 * #define	MPU6050_ACCEL_YOUT_L	0x3E
 * #define	MPU6050_ACCEL_ZOUT_H	0x3F
 * #define	MPU6050_ACCEL_ZOUT_L	0x40
 * #define	MPU6050_TEMP_OUT_H		0x41
 * #define	MPU6050_TEMP_OUT_L		0x42
 * #define	MPU6050_GYRO_XOUT_H		0x43
 * #define	MPU6050_GYRO_XOUT_L		0x44
 * #define	MPU6050_GYRO_YOUT_H		0x45
 * #define	MPU6050_GYRO_YOUT_L		0x46
 * #define	MPU6050_GYRO_ZOUT_H		0x47
 * #define	MPU6050_GYRO_ZOUT_L		0x48
 *
 * 如果想要获取数据，就不断 获取 这些寄存器里的值
 *
 *
 */
 /**
  * 1、初始化 I2C 时钟；
  *     RCC_APB1Periph_I2C1、RCC_APB1Periph_I2C2 都是 APB1 的外设；
  * 2、开启GPIO 的时钟； 这里使用  GPIOC GPIO_Pin_15 GPIO_Pin_14
  * 3、初始化 GPIO_Pin_15 GPIO_Pin_14
  * 4、初始化 I2C2 ； 配置 结构体
  *
  */
void MPU6050_Init(void){
    //customI2C_Init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;// 复用开漏 输出 ； 开漏输出技能输出 也能输入； 复用是因为 I2C的控制权 交给硬件外设；
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //初始化 I2C2
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//
    I2C_InitStructure.I2C_ClockSpeed = 50000;//50000 标志速度； 0～100kHZ 标准 100KHZ～ 400kHz 快速
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//时钟占空比 I2C_DutyCycle_16_9 （16:9） I2C_DutyCycle_2（2:1）； 快速模式下 增加了低电平 的比例；由于低电平数据变化，需要一定的时间来翻转 波形； 高电平读取；
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;// ACK位
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//作为 从机 响应 几位地址； I2C_AcknowledgedAddress_7bit （7 位）；
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;//指定 STM32 的自身地址； 作为从机使用；I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit 就要写一个 7位 地址

    I2C_Init(I2C2, &I2C_InitStructure);
    //使能
    I2C_Cmd(I2C2, ENABLE);
    //============
    //配置电源管理寄存器 1
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    // 配置电源管理寄存器 1
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    // 采样 预分频
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    // 配置寄存器
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    // 陀螺仪 配置
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    // 加速度计配置寄存器
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);

}


/**
 * 这里要返回 6 个 数据
 * 但是 C语言中值让 返回 一个值，所以要特殊 处理
 *
 * 这里使用指针的地址传递
 *  分别 读取6 个轴数据寄存器 的高位 和  低位
 *  拼接成 16  位 数据，再通过针变量 返回
 *
 *  =======
 *  也可使用 连续读取 寄存器 ， 因为读取一个 地址会自增
 *
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ){
    //uint8_t DataH, DataL;
    uint16_t DataH, DataL;
    // 加速度 X 轴
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    // 把读到的数据 通过 指针 返回回去
    *AccX = (DataH << 8) | DataL;// 获得了 16 位数据
    // 加速度 Y 轴
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;
    // 加速度 Z 轴
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;
    // 陀螺仪 X 轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;
    // 陀螺仪 Y 轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;
    // 陀螺仪 Z 轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;


}



