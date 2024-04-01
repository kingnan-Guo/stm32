/**
 *
 *
 *   1、开启 RCC 时钟 ；包括ADC 和 GPIO
 *  2、ADCLOCK 的 分频器 也要配置一下
 *  3、GPIO 配置成模拟输入的 模式 AIN
 *  4、配置 多路开关； 把左边的通道 接入到 右边的规则组中
 *  5、配置 ADC 转换器 ；使用结构体配置 ；
 *      包括： a、ADC 是单次转换还是连续转换、
 *            b、扫描还是非扫描、
 *            c、有几个通道
 *            d 左对齐还是又对齐
 *  6、如果需要模拟看门狗，那么 又 阈值 和 检测通道的可以配置；
 *  7、如果 开启中断 ，那么在 ITConfig 函数开启对应的中断
 *  8、最终 在NVIC 里配置 优先级 那么就可以 开启中断了
 *
 *  9、开关 控制 ADC_Cmd 开启ADC
 *  10、 根据手册的建议 ， 可以对ADC 进行 校准，这样 可以 减少 误差； ADC 初始化后 依次调用 ADC_ResetCalibration ADC_GetResetCalibrationStatus ADC_StartCalibration ADC_GetCalibrationStatus
 *  11、 在ADC 工作过程中 ，可以软件触发、读取结果、
 *
 *
 * ===========================================
 *
 *
 *  RCC.h 中 包含 RCC_ADCCLKConfig ； 可以配置 ADCCLock 的分频
 *
 *
 * ===================================
 *
 * 添加 ADC的 DMA的 功能
 *
 * ADC的扫描模式 + DNA数据转运
 * 1、首先扫描 PA0～PA2 这三个通道
*/
#include "stm32f10x.h"

uint16_t AD_DMA_Value[4];


void AD_DMA_Init(void){
    // 开启 ADC1 的时钟， ADC 都是APB2上的设备
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // GPIO A
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


    // 开启DMA的时钟
    //DMA 是AHB 总线设备所以要用AHB开启时钟
    // RCC_AHBPeriph_DMA1  ; ENABLE 开启时 DMA1的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);



    // ADCClock 需要配置 分频配置
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 6分频 12MHZ   ； 72 / 6 = 12

    // GPIO A0  配置 成 模拟输入的引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;// GPIO_Mode_AIN 用于 ADC 上； 模拟输入; 在 AIN 模式下 ；GPIO 口 是 无效的； 断开GPIO； 防止GPIO 的输入输出对我 模拟电压造成干扰； AIN 就是 ADC 的专属 模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ADC 规则转换中
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 4, ADC_SampleTime_55Cycles5);


    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//  独立模式 ADC1 ADC2 各自转换各自的
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// 数据对齐 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 外部触发选择 ADC_ExternalTrigConv_None  不使用外部出发 使用内部软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;// 连续转换模式 还是 单次转换； 单次转换； 扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;// 扫描转换模式 扫描还是 非扫描； 非扫描
    ADC_InitStructure.ADC_NbrOfChannel = 4;// 通道数目 1～16 之间； 放了4跳数据在

    ADC_Init(ADC1, &ADC_InitStructure);




    // 初始化DMA
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;// 起始地址 基地址  32位  eg:0x2000 0000；DR 地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 数据宽度 想要存储AR寄存器， DR寄存器低 16数据线； 所以低16 选择 halfWord 以半字 15位来转运
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 是否自增；DIsable在Disabale 因为规则； 始终转运同一个位置的数据
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_DMA_Value;//粗初期站点 基地址 ； 结束地址 ; AD_DMA_Value
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 数据宽度 DMA_MemoryDataSize_HalfWord 半字节
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 存储器 站点地址自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向 指定外设站点是远端 还是目的地； DMA_DIR_PeripheralDST 外设站点作为DST 目的地，存储器到外设 ；DMA_DIR_PeripheralSRC 外设站点作为src 数据源； 当前案例 外设-> 存储器
    DMA_InitStructure.DMA_BufferSize = 4;// 缓存区大小 ，传输计数器； 外设单元等于外设数据宽度或者存储器数据宽度； 0～65535； 有四个外设通道
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 传输模式、是否使用 自动重装；DMA_Mode_Normal：  正常模式 传输计数器不自动重装; DMA_Mode_Circular:循环模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 软硬件触发 DMA_M2M_Disable 使用 硬件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
    // DMAy_Channelx 使用 DMA1 和 通道 1： y： 1～2 ； 写：1～7
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);


    // 开启 ADC DMA的
    ADC_DMACmd(ADC1, ENABLE);

    // 开启 ADC
    ADC_Cmd(ADC1, ENABLE);

    // 可以在这里开启 看门狗

    // 校准
    ADC_ResetCalibration(ADC1);//复位校准
    //ADC_GetResetCalibrationStatus 返回复位校准状态
    // 一旦 硬件 清零 那么 跳出 while
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);// 循环等到 复位校准 状态 为 1； 获取 就是 CR2_RSTCAL_SET :CR2寄存器的 RSTCAL 的值； 由 软件设置 硬件清除，在校验寄存器被初始化后 该位 将被清除；执行顺序 即使 软件置为 1 硬件就开始 校验 校验完成后 硬件 置 0；

    // 启动 校准 内部电路自动校准
    ADC_StartCalibration(ADC1);
    //等待 校准 完成
    //ADC_GetCalibrationStatus 获取校准标志位
    while (ADC_GetCalibrationStatus(ADC1) == SET);


}

/**
 * 1、软件触发 转换 ADC_SoftwareStartConvCmd
 * 2、等待 转换完成 EOC 置 1
 * 3、 对去 ADC 数据 寄存器
 * @return
 */
uint16_t AD_DMA_GetValue(void){
    // 在触发ADC 之前 ，需要重新写入一下 传输计数器

    DMA_Cmd(DMA1_Channel1, DISABLE);
    //DMA1_Channel1 ; 给传输计数器写入的值，这里要获取一下初始化的参数
    DMA_SetCurrDataCounter(DMA1_Channel1, 4);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // 软件触发 函数
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    // 这里添加 DMA是否完成的 判定
    // DMA1_FLAG_TC1 转运完成标志位; 转运完成后 标志位置 1
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
    DMA_ClearFlag(DMA1_FLAG_TC1); //需要手动 清除

}









