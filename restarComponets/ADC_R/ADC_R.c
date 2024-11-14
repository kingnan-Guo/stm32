//
// Created by 90175 on 2024/11/14.
//

// 1、开启 RCC GPIO 时钟
// 2、开启 RCC 时钟  ADC
// 3、初始化 GPIO ， 模式：模拟输入
// 4、 配置多路开关
// 5、把通道接入到 规则组列表里
// 6、配置ADC 转换器： 单次转换 、连续转换、扫描 非扫描、有几个通道 、数据对齐方式
// 7、 开关控制 ADC_Cmd
// 8、 对 ADC 进行转换



// 可选：
// 开启模拟中断： ITConfig 开启
// NVIC 优先级
// 模拟看门狗


// 计划使用PA0
#include "stm32f10x.h"
#include "ADC_R.h"

void ADC_R_INIT(){
    // GPIO_A  初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//  模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



    // ADC 配置 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    // RCC ADC Clock 传进 ADC 模拟数字转换器的分频，6分频 12 MHZ
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    // 配置 规则组的 输入通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

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


uint16_t ADC_R_GetValue(){

    // ADC_RegularChannelConfig();

    // 软件触发 转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 等大 EOC 标志位
    while (ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);// EOC 由软件清除 或 ADC_DR 时 清除
    // 读取寄存器的值
    return ADC_GetConversionValue(ADC1);// 读取 ADCx 的寄存器 会自动清除 EOC 标志位
}
