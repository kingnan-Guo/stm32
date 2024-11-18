//
// Created by 90175 on 2024/11/14.
//
//步骤
//1、 RCC 开启  DMA1 时钟
//
//2、初始化 DMA1
//  DMA1 配置
//      转运的 起始地址 目标地址 是否自增
//      目标地址 是否自增
//      方向
//      传输计数器 ： 配置 自增 数量 传输计数器， 是否 自动重装 重装的值
//
//3、M2M(Memery to Memery) 控制
//  触发源 , 可以是 软件 也可以是 硬件
//
//4、通道优先级
//
//
//5、 DMA_Cmd() 使能 开始转运信号


// 要修在转运过程中修改 AddrStar
// 如果 要修改 转运的地址 需要先 失能 , 修改 地址， 然后再 使能， 再次 转运



#include "stm32f10x.h"
#include "DMA_R.h"
#include "OLED.h"
#include "delay.h"
uint16_t MY_DMA_BufferSize;
void DMA_R_INIT(uint32_t AddrStar, uint32_t AddrEnd, uint16_t size){
    MY_DMA_BufferSize = size;
    // DMA 可以控制  AHB 总线 ， AHB 总线也可以修改 DMA 的寄存器
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


    DMA_InitTypeDef DMA_InitStruct;
    DMA_StructInit(&DMA_InitStruct);

    DMA_InitStruct.DMA_PeripheralBaseAddr = AddrStar;// 外设的地址
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设大小 8 位 一个字节
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;//  是否自增
    DMA_InitStruct.DMA_MemoryBaseAddr = AddrEnd;//寄存器地址
    DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;//
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;// 是否自增
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;// 转运方向 /传输方向 指定外设站点是远端 还是目的地； DMA_DIR_PeripheralDST 外设站点作为DST 目的地，存储器到外设 ；DMA_DIR_PeripheralSRC 外设站点作为src 数据源； 当前案例 外设-> 存储器
    DMA_InitStruct.DMA_BufferSize = size;// 转运计数器
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;// 是否 自动重装
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;// DMA_M2M_Enable 软件触发 ； DMA_M2M_Disable  硬件触发
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;// 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    DMA_Cmd(DMA1_Channel1, DISABLE);
}


// 开始 转运
void DMA_R_TRANSFER(){
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, MY_DMA_BufferSize);
    // 开始 转运
    DMA_Cmd(DMA1_Channel1, ENABLE);
    // 获取中断标志位
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);// DMA1_FLAG_TC1 转运完成
    // 清除 标志位
    DMA_ClearFlag(DMA1_FLAG_TC1);
}


// 把 DataA 转到 DataB 中 ，是复制
uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0,0,0,0};
void SHOW_DATA_INIT(){


    DMA_R_INIT((uint32_t)DataA, (uint32_t)DataB, 4);
    DMA_R_TRANSFER();

    OLED_ShowString(1, 1, "DataA");
    OLED_ShowString(3, 1, "DataB");

    OLED_ShowHexNum(1, 8,  (uint32_t)DataA, 8);
    OLED_ShowHexNum(3, 8,  (uint32_t)DataB, 8);




}

 void SHOW_DATA_WHILE(){
     DataA[0] ++;
     DataA[1] ++;
     DataA[2] ++;
     DataA[3] ++;


     OLED_ShowHexNum(2, 1, DataA[0], 2);
     OLED_ShowHexNum(2, 4, DataA[1], 2);
     OLED_ShowHexNum(2, 7, DataA[2], 2);
     OLED_ShowHexNum(2, 10, DataA[3], 2);

     OLED_ShowHexNum(4, 1, DataB[0], 2);
     OLED_ShowHexNum(4, 4, DataB[1], 2);
     OLED_ShowHexNum(4, 7, DataB[2], 2);
     OLED_ShowHexNum(4, 10, DataB[3], 2);
     Delay_ms(1000);
     DMA_R_TRANSFER();



     OLED_ShowHexNum(2, 1, DataA[0], 2);
     OLED_ShowHexNum(2, 4, DataA[1], 2);
     OLED_ShowHexNum(2, 7, DataA[2], 2);
     OLED_ShowHexNum(2, 10, DataA[3], 2);

     OLED_ShowHexNum(4, 1, DataB[0], 2);
     OLED_ShowHexNum(4, 4, DataB[1], 2);
     OLED_ShowHexNum(4, 7, DataB[2], 2);
     OLED_ShowHexNum(4, 10, DataB[3], 2);


     Delay_ms(1000);
}




//#include "DMA_R.h"
//
//int main(void) {
//    OLED_Init();
//    SHOW_DATA_INIT();
//    while(1) {
//        SHOW_DATA_WHILE();
//    }
//}

