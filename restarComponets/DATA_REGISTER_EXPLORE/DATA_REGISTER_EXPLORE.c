//
// Created by 90175 on 2024/11/15.


#include "stm32f10x.h"
#include "DATA_REGISTER_EXPLORE.h"
#include "OLED.h"

#define ADC1_DR (uint32_t *)0x4001244c;

int testData = 0x66;// SRAM 内存数据 存入 0x2000 0000 开始
const int ConstData = 0x88;// const 常量 只能读 不能写， 地址时  0x8000 0000, 根据地址判断 是储存在 FLASH中;FLASH 里的数据 只能读 不能写， 储存 程序代码 和 常量数据
void OLED_SHOW_DATA_ADRR(){
    OLED_ShowHexNum(1, 1, testData, 9);
    OLED_ShowHexNum(2,1,(uint32_t)&testData,10);
    OLED_ShowHexNum(3,1,(uint32_t)&ConstData,10);
}





// 外设寄存器地址
// 外设寄存器 地址固定 ;
void GET_REGISER_ADRR(){
    // ADC1->DR;// 访问 ADC1 的 DR 寄存器; SRAM 区域 ;地址是 0x4001 244C
    // ADC 的地址 是  0x4001 2400;
    // DR 寄存器的 偏移量 是 4C
    // ADC1_BASE 是 基地址 （起始地址 0x4001 2400， 偏移是 0x0000 2400）， ADC_TypeDef 是 指针
    // ADC1 是结构体指针 ，类型是 ADC_TypeDef， 指向的是 ADC1 的起始地址，访问 结构体成员 就是 指针 + 偏移

    OLED_ShowHexNum(1,1,(uint32_t)&(ADC1->DR),10);



    // *ADC1_DR
}

// 预研 malloc 如何使用
void TEST_MALLOC(){

}



//=================
//#include "stm32f10x.h"
//#include "OLED.h"
//#include "delay.h"
//#include <stdio.h>
//#include "DATA_REGISTER_EXPLORE.h"
//int main(void) {
//    OLED_Init();
//    while(1) {
//        OLED_SHOW_DATA_ADRR();
//    }
//}
