//
// Created by 90175 on 2024/11/18.
//

// 初始化 GPIO
// 模拟I2C 使用延时 模拟 电平


#include "stm32f10x.h"
#include "SOFTWARE_I2C.h"
#include "delay.h"
#include "OLED.h"

#define I2C_PORT    GPIOB
#define SCL_PIN     GPIO_Pin_6
#define SDA_PIN     GPIO_Pin_7


void SOFTWARE_I2C_INIT(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;// 开漏 输出 可以输入
    GPIO_InitStruct.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_PORT, &GPIO_InitStruct);

    // 设置 GPIO 高电平，此时 输入 空闲状态
    GPIO_SetBits(I2C_PORT, SCL_PIN | SDA_PIN);
}


void SOFTWARE_I2C_W_SCL(uint8_t bitValue){
    GPIO_WriteBit(I2C_PORT, SCL_PIN, (BitAction)bitValue);
    Delay_us(10);
}

void SOFTWARE_I2C_W_SDA(uint8_t bitValue){
    GPIO_WriteBit(GPIOB, SDA_PIN, (BitAction)bitValue);
    Delay_us(10);
}

uint8_t SOFTWARE_I2C_R_SDA(){
    uint8_t bitValue;
    bitValue = GPIO_ReadInputDataBit(I2C_PORT, SDA_PIN);
    Delay_us(10);
    return bitValue;
}

// SCL \___          ___/
// SDA   \__       __/
//下拉
void SOFTWARE_I2C_START(){
    SOFTWARE_I2C_W_SCL(1);
    SOFTWARE_I2C_W_SDA(1);


    SOFTWARE_I2C_W_SDA(0);
    SOFTWARE_I2C_W_SCL(0);
}

void SOFTWARE_I2C_STOP(){
    // SOFTWARE_I2C_W_SCL(0);
    SOFTWARE_I2C_W_SDA(0);

    SOFTWARE_I2C_W_SCL(1);
    SOFTWARE_I2C_W_SDA(1);
}


// 发送数据 高位先行
void SOFTWARE_I2C_SEND_BYTE(uint8_t BYTE){
    for (int i = 0; i < 8; ++i) {
        // BYTE << i | 00000001;
        SOFTWARE_I2C_W_SDA(BYTE & (0x80 >> i));
        SOFTWARE_I2C_W_SCL(1);
        SOFTWARE_I2C_W_SCL(0);
    }
}

// 发送  ACK
// 软件 模拟 ACK 低电平 代表 已经收到数据
void SOFTWARE_I2C_SEND_ACK(uint8_t BYTE){
    SOFTWARE_I2C_W_SDA(BYTE);
    SOFTWARE_I2C_W_SCL(1);
    SOFTWARE_I2C_W_SCL(0);
}

// 接收 数据
// 高位先行
uint8_t SOFTWARE_I2C_RECEIVE_BYTE(){
    uint8_t BYTE = 0x00;
    uint8_t i = 0x00;
    SOFTWARE_I2C_W_SDA(1);// 释放 SDA 开始接收
    for ( i = 0; i < 8; ++i) {
        SOFTWARE_I2C_W_SCL(1);
        // BYTE = BYTE | SOFTWARE_I2C_R_SDA() & (0x80 >> i);
        if(SOFTWARE_I2C_R_SDA() == 1){
            BYTE |= (0x80 >> i);
        }
        SOFTWARE_I2C_W_SCL(0);
    }
    return BYTE;
}

// 接收  ACK
uint8_t SOFTWARE_I2C_RECEIVE_ACK(){
    uint8_t AckBit = 0x00;
    SOFTWARE_I2C_W_SDA(1);
    SOFTWARE_I2C_W_SCL(1);
    AckBit = SOFTWARE_I2C_R_SDA();
    SOFTWARE_I2C_W_SCL(0);
    return AckBit;
}


void SOFTWARE_I2C_main(){
    uint8_t ID;
    SOFTWARE_I2C_INIT();
    SOFTWARE_I2C_START();

    // 发送 MPU6050 地址 写
    SOFTWARE_I2C_SEND_BYTE(0xD0);
    // 接收 ACK
    uint8_t ACK_0 = SOFTWARE_I2C_RECEIVE_ACK();
    SOFTWARE_I2C_SEND_BYTE(0x75);
    SOFTWARE_I2C_STOP();

    SOFTWARE_I2C_START();
    ID = SOFTWARE_I2C_RECEIVE_BYTE();
    SOFTWARE_I2C_STOP();

    OLED_ShowString(1,1, "ID ");
    OLED_ShowHexNum(1,4, ID, 6);
    OLED_ShowNum(2,1, ACK_0, 5);

}
