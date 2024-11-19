//
// Created by 90175 on 2024/11/19.
//

// 在 STM32 的 I2C 硬件 中 ： MPU6050_ADDRESS 最低为 置  1 是 读

#include "stm32f10x.h"
#include "delay.h"
#include "HARDWARE_CONTROL_MPU6050.h"
#include "MPU6050_Reg.h"
#include "OLED.h"

#define MPU6050_ADDRESS     0xD0


void HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(uint8_t REGISTER_ADDRESS, uint8_t DATA){
    // 起始
    I2C_GenerateSTART(I2C2, ENABLE);
    // 检测标志位
    // I2C_CheckEvent 状态监控
    // I2C_EVENT_MASTER_MODE_SELECT 主机模式 选择； 因为 STM32  默认为 从机， 发送 其实条件后 变为 主机
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

    // 发送从机 地址 接收应答； 接收应答并不需要单独的 函数 ，这里自带 接收应答的过程， 同样 接收数据也带了发送应答的过程； 应答错误 会通过 标志位 或者 中断 提示
    // 直接向  DR 寄存器 写入 一个字节 ;
    // I2C_Direction_Transmitter :  地址 最低为 清零 ； 目的是发送 所以 选择 这个
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);


    // 这里 会产生 EV6 事件 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  发送数据 已选择 ; I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED 接收事件已选择
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);


    // EV8_1

    // 发送数据
    I2C_SendData(I2C2, REGISTER_ADDRESS);
    // 发送后 需要等待 EV8 事件； 字节正在发送
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
    // 然后可以直接写入 下一句；
    I2C_SendData(I2C2, DATA);

    // 等待 EV8_2 事件
    // BTF 标志位 为 1 ， 移位完成了，并且没有新的数据完成了 所以 志标位
    // 最后要等 硬件 把两级缓存 所有数据都清空 ，才开始产生 终止条件
    // I2C_EVENT_MASTER_BYTE_TRANSMITTED : EV8_2
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);


    // 终止
    I2C_GenerateSTOP(I2C1, ENABLE);
}

// 读 寄存器
uint8_t HARDWARE_CONTROL_MPU6050_READ_REGISTER(uint8_t REGISTER_ADDRESS){
    uint8_t DATA;

    // 起始
    I2C_GenerateSTART(I2C2, ENABLE);

    // 检测标志位 EV5
    // I2C_CheckEvent 状态监控
    // I2C_EVENT_MASTER_MODE_SELECT 主机模式 选择； 因为 STM32  默认为 从机， 发送 其实条件后 变为 主机
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);

    // 发送从机 地址 接收应答； 接收应答并不需要单独的 函数 ，这里自带 接收应答的过程， 同样 接收数据也带了发送应答的过程； 应答错误 会通过 标志位 或者 中断 提示
    // 直接向  DR 寄存器 写入 一个字节 ;
    // I2C_Direction_Transmitter :  地址 最低为 清零 ； 目的是发送 所以 选择 这个
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);

    // 这里 会产生 EV6 事件 I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  发送数据 已选择 ; I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED 接收事件已选择
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);

    // 发送数据
    I2C_SendData(I2C2, REGISTER_ADDRESS);
    // 发送后 需要等待 EV8 事件； 字节正在发送
    // while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);// 数据发送中： 这个也可以 替换下面一句
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);// 等待 数据发送完

    // === 再次 开始 ===

    I2C_GenerateSTART(I2C2, ENABLE);
    // 起始 条件后  等待 EV5
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
    // 发送 从机地址; I2C_Direction_Receiver 接收方向 ，自动 把 MPU6050_ADDRESS 最低为 置 1
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);

    //寻址 之后 EV6 事件
    // 但是 主机 接收 的EV6 并不是主机发送的EV6
    // I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED 主机发送的 EV6
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);

    // 只接受 一个字节 ： 接收之前  ACK 置 0 ；stop 置 1， 因为接收完成 来不及
    // 接收 多个字节 ： 在 最后一个  EV7 的时候 之前可以配置  ACK 置 0 ；stop 置 1 就可以
    // 这里接收 一个字节
    I2C_AcknowledgeConfig(I2C2, DISABLE);// ACK = 0
    I2C_GenerateSTOP(I2C1, ENABLE);// STOP = 1

    // 等待 EV7 事件； EV7 事件产生后 一个字节的数据就已经 在 DR 里了，然后读取DR 就可以 获得到数据
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);

    // 最后 接收一位 数据
    DATA = I2C_ReceiveData(I2C2);

    // 最后把 ACK 置 回  1； 恢复到初始状态
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    return DATA;
}


void HARDWARE_CONTROL_MPU6050_INIT(){


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;// 复用开漏 输出 ； 开漏输出技能输出 也能输入； 复用是因为 I2C的控制权 交给硬件外设；
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    I2C_InitTypeDef I2C_InitStruct;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;//I2C 模式 ，
    I2C_InitStruct.I2C_ClockSpeed = 50000;// 时钟频率 0~100KHZ 1000KHZ ~ 400KHZ
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;// 占空比 在 100KHZ 以下 占空比 是 50% 50%
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;// 7 位
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_Init(I2C2, &I2C_InitStruct);


    // =====
    //配置电源管理寄存器 1
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_PWR_MGMT_1, 0x01);
    // 配置电源管理寄存器 1
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_PWR_MGMT_2, 0x00);
    // 采样 预分频
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_SMPLRT_DIV, 0x09);
    // 配置寄存器
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_CONFIG, 0x06);
    // 陀螺仪 配置
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_GYRO_CONFIG, 0x18);
    // 加速度计配置寄存器
    HARDWARE_CONTROL_MPU6050_WRITE_REGISTER(MPU6050_ACCEL_CONFIG, 0x18);
}





void HARDWARE_CONTROL_MPU6050_GETDATA(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ){
    //uint8_t DataH, DataL;
    uint16_t DataH, DataL;
    // 加速度 X 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_XOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_XOUT_L);
    // 把读到的数据 通过 指针 返回回去
    *AccX = (DataH << 8) | DataL;// 获得了 16 位数据
    // 加速度 Y 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_YOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;
    // 加速度 Z 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_ZOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;
    // 陀螺仪 X 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_XOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;
    // 陀螺仪 Y 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_YOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;
    // 陀螺仪 Z 轴
    DataH = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_ZOUT_H);
    DataL = HARDWARE_CONTROL_MPU6050_READ_REGISTER(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;


}



// ================================================

int16_t AX, AY, AZ, GX, GY, GZ;// 用于 记录 加速度计 陀螺仪的值
void HARDWARE_CONTROL_MPU6050_MAIN(){
    OLED_Init();
    uint8_t ID;

    HARDWARE_CONTROL_MPU6050_INIT();
    // 读取 ID 寄存器地址 0x75
    ID = HARDWARE_CONTROL_MPU6050_READ_REGISTER(0x75);
    OLED_ShowString(1, 1, "ID ");
    OLED_ShowHexNum(1, 4, ID, 2);
}

void HARDWARE_CONTROL_MPU6050_MAIN_WHILE() {
    HARDWARE_CONTROL_MPU6050_GETDATA(&AX, &AY, &AZ, &GX, &GY, &GZ);
    OLED_ShowSignedNum(2, 1, AX, 5);
    OLED_ShowSignedNum(3, 1, AY, 5);
    OLED_ShowSignedNum(4, 1, AZ, 5);
    OLED_ShowSignedNum(2, 8, GX, 5);
    OLED_ShowSignedNum(3, 8, GY, 5);
    OLED_ShowSignedNum(4, 8, GZ, 5);
}

