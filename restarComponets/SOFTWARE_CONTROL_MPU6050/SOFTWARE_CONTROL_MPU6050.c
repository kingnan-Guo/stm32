//
// Created by 90175 on 2024/11/18.
//
#include "stm32f10x.h"
#include "SOFTWARE_CONTROL_MPU6050.h"
#include "SOFTWARE_I2C.h"
#include "delay.h"
#include "OLED.h"


#define MPU6050_ADDRESS     0xD0 // 1101 0000

void SOFTWARE_CONTROL_MPU6050_INIT(){
    SOFTWARE_I2C_INIT();
}

// 写
void SOFTWARE_CONTROL_MPU6050_WRITE_REGISTER(uint8_t REGISTER_ADDRESS, uint8_t DATA){
    uint8_t ACK;
    SOFTWARE_I2C_START();

    // 发送 数据
    SOFTWARE_I2C_SEND_BYTE(MPU6050_ADDRESS);
    ACK = SOFTWARE_I2C_RECEIVE_ACK();// 接收应答

    // 发送 寄存器地址
    SOFTWARE_I2C_SEND_BYTE(REGISTER_ADDRESS);
    ACK = SOFTWARE_I2C_RECEIVE_BYTE();// 接收应答

    // 发送 数据
    SOFTWARE_I2C_SEND_BYTE(DATA);
    ACK = SOFTWARE_I2C_RECEIVE_ACK();// 接收应答

    SOFTWARE_I2C_STOP();
}


// 读
uint8_t SOFTWARE_CONTROL_MPU6050_READ_REGISTER(uint8_t REGISTER_ADDRESS){
    uint8_t ACK;
    uint8_t DATA;
    SOFTWARE_I2C_START();

    // 发送 数据
    SOFTWARE_I2C_SEND_BYTE(MPU6050_ADDRESS);
    ACK = SOFTWARE_I2C_RECEIVE_ACK();// 接收应答

    // 发送 寄存器地址
    SOFTWARE_I2C_SEND_BYTE(REGISTER_ADDRESS);
    ACK = SOFTWARE_I2C_RECEIVE_ACK();// 接收应答

    SOFTWARE_I2C_STOP();

    // ========== 开始接收数据 ===============
    SOFTWARE_I2C_START();

    // 写 数据
    SOFTWARE_I2C_SEND_BYTE(MPU6050_ADDRESS | 0x01);
    // ACK = SOFTWARE_I2C_RECEIVE_ACK();// 接收应答

    DATA = SOFTWARE_I2C_RECEIVE_BYTE();// 接收数据

    // 主机发送 ACK
    SOFTWARE_I2C_SEND_ACK(1);

    SOFTWARE_I2C_STOP();

    return DATA;
}


void SOFTWARE_CONTROL_MAIN(){
    uint8_t ID;

    SOFTWARE_CONTROL_MPU6050_INIT();
    // 读取 ID
    ID = SOFTWARE_CONTROL_MPU6050_READ_REGISTER(0x75);
    OLED_ShowString(1,1, "ID ");
    OLED_ShowHexNum(1,4, ID, 6);


    // 首先 解除 睡眠模式 0x68 写入 0x00
    SOFTWARE_CONTROL_MPU6050_WRITE_REGISTER(0x68, 0x00);


    // 测试写入数据
    // 在 0x19 中写入 AA ; 0x19 是 采样分频寄存器 SMPLRT_DIV
    // 读取 0x19

    SOFTWARE_CONTROL_MPU6050_WRITE_REGISTER(0x19, 0xAA);

    uint8_t SMPLRT_DIV= SOFTWARE_CONTROL_MPU6050_READ_REGISTER(0x19);

    OLED_ShowString(2,1, "SMPLRT_DIV ");
    OLED_ShowHexNum(2,12, SMPLRT_DIV, 2);

}


