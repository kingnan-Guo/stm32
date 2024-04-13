#include "stm32f10x.h"
#include "delay.h"
#include "customI2C.h"
#include "MPU6050_Reg.h"


#define MPU6050_ADDRESS     0xD0
// 使用 宏定 将 寄存器的地址 使用  字符串来表示





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
    uint8_t ACK;
    customI2C_Start();
    customI2C_SendByte(MPU6050_ADDRESS);// 1101 0000 写入操作
    // 接收应答
    ACK = customI2C_ReceiveAckByte();
    // 继续发送下一个字节， 寄存器的地址； 进行写入操作
    customI2C_SendByte(RegAddress);
    ACK = customI2C_ReceiveAckByte();
    // 要写如 寄存器 中的数据
    customI2C_SendByte(Data);
    ACK = customI2C_ReceiveAckByte();
    // 停止
    customI2C_Stop();
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
    uint8_t ACK;
    uint8_t Data;
    customI2C_Start();
    customI2C_SendByte(MPU6050_ADDRESS);// 1101 0000 写入操作
    // 接收应答
    ACK = customI2C_ReceiveAckByte();
    // 继续发送下一个字节， 寄存器的地址； 进行写入操作
    customI2C_SendByte(RegAddress);
    ACK = customI2C_ReceiveAckByte();

    // ========== 开始 接收数据 ========

    customI2C_Start();
    // 寄存器的地址； 进行 读取 入操作; 1101 0001
    customI2C_SendByte((MPU6050_ADDRESS|0x01));
    ACK = customI2C_ReceiveAckByte();
    // 接收应答 之后 总线控制权 就正式交给 从机
    // 主机 开始接收 数据
    Data =  customI2C_ReceiveByte();
    customI2C_SendACK(1);// 主机给 应答 1 ，也就是 释放电平，从机没有收到 低电平 ，所以不会再继续 发送数据
    customI2C_Stop();

    return Data;
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
void MPU6050_Init(void){
    customI2C_Init();
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

