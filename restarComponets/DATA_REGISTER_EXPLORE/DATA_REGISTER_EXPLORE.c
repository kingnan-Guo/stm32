//
// Created by 90175 on 2024/11/15.


#include "stm32f10x.h"
#include "DATA_REGISTER_EXPLORE.h"
#include "OLED.h"



int testData = 0x66;// RAM 内存数据 存入 0x2000 0000 开始
const int ConstData = 0x88;//
void OLED_SHOW_DATA_ADRR(){
    OLED_ShowHexNum(1, 1, testData, 9);
    OLED_ShowHexNum(2,1,(uint32_t)&testData,10);
    OLED_ShowHexNum(3,1,(uint32_t)&ConstData,10);
}

// 预研 malloc 如何使用
void TEST_MALLOC(){

}

//=================

//#include "DATA_REGISTER_EXPLORE.h"
//int main(void) {
//    OLED_Init();
//    while(1) {
//        OLED_SHOW_DATA_ADRR();
//    }
//}
