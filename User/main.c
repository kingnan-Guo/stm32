#include "stm32f10x.h"

#include "OLED.h"
//#include "delay.h"
#include "MPU6050.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();


    uint8_t ID;
    int16_t AX, AY, AZ, GX, GY, GZ;// 用于 记录 加速度计 陀螺仪的值
    MPU6050_Init();
    // 读取 ID 寄存器地址 0x75
    ID = MPU6050_ReadReg(0x75);
    OLED_ShowString(1, 1, "ID ");
    OLED_ShowHexNum(1, 4, ID, 2);

    /**
     * 写入 寄存器
     * 首先 接触 睡眠 模式
     *
     *  睡眠模式在  电源管理寄存器 的 第二位 控制的
     *  地址： 0x6B
     *  写入的 数据： 0x00； 在电源管理寄存器 1 写入 0x00， 解除 睡眠模式
     *
     *
     */
    //MPU6050_WriteReg(0x6B, 0x00);

    /**
     * 1、写入 采样分频寄存器 SMPLRT_DIV
     *  地址 ： 0x19
     *  数据： 0xAA
     * 2、写入后读取， 可以读取 出来 AA
     */
    //MPU6050_WriteReg(0x19, 0x01);
    //uint8_t SMPLRT_DIV = MPU6050_ReadReg(0x19);
    //OLED_ShowString(2, 1, "SMPLRT_DIV ");
    //OLED_ShowHexNum(2, 12, SMPLRT_DIV, 2);


    while(1) {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

        OLED_ShowSignedNum(2, 1, AX, 5);
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);


    }
}



/**
 *
 *
 *
 */

