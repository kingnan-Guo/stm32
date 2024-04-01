#include "stm32f10x.h"

uint16_t customDMA_Size;

void customDMA_Init(uint32_t AddrStar, uint32_t AddrEnd, uint16_t Size){
    customDMA_Size = Size;
    //DMA 是AHB 总线设备所以要用AHB开启时钟
    // RCC_AHBPeriph_DMA1  ; ENABLE 开启时 DMA1的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 初始化DMA
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrStar;// 起始地址 基地址  32位  eg:0x2000 0000
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// 数据宽度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;// 是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrEnd;// 结束地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;// 数据宽度
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 存储器 站点地址自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向 指定外设站点是远端 还是目的地； DMA_DIR_PeripheralDST 外设站点作为DST 目的地，存储器到外设 ；DMA_DIR_PeripheralSRC 外设站点作为src 数据源； 当前案例 外设-> 存储器
    DMA_InitStructure.DMA_BufferSize = Size;// 缓存区大小 ，传输计数器； 外设单元等于外设数据宽度或者存储器数据宽度； 0～65535
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 传输模式、是否使用 自动重装；DMA_Mode_Normal：  正常模式 传输计数器不自动重装
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;// 软硬件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
    // DMAy_Channelx 使用 DMA1 和 通道 1： y： 1～2 ； 写：1～7
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, DISABLE);
}

/**
 * 要修在转运过程中修改 AddrStar
 * 1、DMA1 先失能
 * 2、修改值
 * 3、再次使能
 * 4、DMA 就会再次转运
 * 5、等待转运完成
 *
 *
 *
 */
void customDMA_Transfer(void){
    // DMA1 先失能
    DMA_Cmd(DMA1_Channel1, DISABLE);
    //DMA1_Channel1 ; 给传输计数器写入的值，这里要获取一下初始化的参数
    DMA_SetCurrDataCounter(DMA1_Channel1, customDMA_Size);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    // DMA1_FLAG_TC1 转运完成标志位; 转运完成后 标志位置 1
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1); //需要手动 清除
}

