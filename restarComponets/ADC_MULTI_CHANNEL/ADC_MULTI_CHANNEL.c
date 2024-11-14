//
// Created by 90175 on 2024/11/14.
//

#include "stm32f10x.h"
#include "ADC_MULTI_CHANNEL.h"

void ADC_MULTI_CHANNEL_INIT(){
    // GPIO_A  初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//  模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADC 配置 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // RCC ADC Clock 传进 ADC 模拟数字转换器的分频，6分频 12 MHZ
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // 配置 规则组的 输入通道
    // ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

    //ADC_Init
    ADC_InitTypeDef ADC_InitStruct;
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;// 模式  ADC_Mode_Independent 独立模式 ADC1 ADC2 各自转换各自的
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;// 连续 还是 单个
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ; // 转运 使用的通道，选择触发器 ，我觉得可以多选，这个是不是 16 个通道选一个 ，咋选值 不知道

    ADC_InitStruct.ADC_NbrOfChannel = 1;// 通道个数
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;// 扫描 非扫描

    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_Cmd(ADC1, ENABLE);


    // 校准 =============== 固定流程
    ADC_ResetCalibration(ADC1);//复位 校准
    // ADC_GetResetCalibrationStatus 返回复位状态
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    // 获取校准标志位
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}


uint16_t ADC_MULTI_CHANNEL_GetValue(uint8_t ADC_Channel){

    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    // 软件触发 转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 等大 EOC 标志位
    while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);// EOC 由软件清除 或 ADC_DR 时 清除
    // 读取寄存器的值
    return ADC_GetConversionValue(ADC1);// 读取 ADCx 的寄存器 会自动清除 EOC 标志位
}




//================


//#include "stm32f10x.h"
//#include "OLED.h"
//#include "delay.h"
//#include <stdio.h>
//#include "ADC_MULTI_CHANNEL.h"
//
//uint16_t ADValue;
//float Voltage;
//int main(void) {
//    OLED_Init();
//    ADC_MULTI_CHANNEL_INIT();
//    while(1) {
//        OLED_ShowNum(1, 1, ADC_MULTI_CHANNEL_GetValue(ADC_Channel_0), 4);
//        OLED_ShowNum(2, 1, ADC_MULTI_CHANNEL_GetValue(ADC_Channel_1), 4);
//    }
//}
