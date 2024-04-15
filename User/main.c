#include "stm32f10x.h"

#include "OLED.h"
//#include "delay.h"
#include "MPU6050.h"

int main(void) {
    // 初始化 I2C 的引脚
    OLED_Init();
//    OLED_ShowString(1, 10, "start ");

    uint8_t ID;
    int16_t AX, AY, AZ, GX, GY, GZ;// 用于 记录 加速度计 陀螺仪的值
    MPU6050_Init();
    // 读取 ID 寄存器地址 0x75
    ID = MPU6050_ReadReg(0x75);
    OLED_ShowString(1, 1, "ID ");
    OLED_ShowHexNum(1, 4, ID, 2);

//    OLED_ShowString(2, 1, "start 2");
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

