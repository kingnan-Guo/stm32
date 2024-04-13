#include "stm32f10x.h"
#include "delay.h"
#include "customI2C.h"


#define MPU6050_ADDRESS     0xD0

void MPU6050_Init(void){
    customI2C_Init();
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

