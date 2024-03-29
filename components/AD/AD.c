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
*/
#include "stm32f10x.h"
void AD_Init(void){
    // 开启 ADC1 的时钟， ADC 都是APB2上的设备
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // GPIO A
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ADCClock 需要配置 分频配置
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 6分频 12MHZ   ； 72 / 6 = 12

    // GPIO A0  配置 成 模拟输入的引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;// GPIO_Mode_AIN 用于 ADC 上； 模拟输入; 在 AIN 模式下 ；GPIO 口 是 无效的； 断开GPIO； 防止GPIO 的输入输出对我 模拟电压造成干扰； AIN 就是 ADC 的专属 模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 选择 规则组的 输入通道 ; 哪个ADCx； ADC_Channel 指定通道；Rank 位置 在第几行(1~ 16 之间)； ADC_SampleTime  通道 采样时间 ，要求不高 随意选择； 需要更快转换 需要小的参数 但不稳定
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);// ADC_SampleTime_55Cycles5 =55.5 个采样 周期
    // ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 设置多个通道

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//  独立模式 ADC1 ADC2 各自转换各自的
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// 数据对齐 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 外部触发选择 ADC_ExternalTrigConv_None  不使用外部出发 使用内部软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;// 连续转换模式 还是 单次转换； 单次转换
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;// 扫描转换模式 扫描还是 非扫描； 非扫描
    ADC_InitStructure.ADC_NbrOfChannel = 1;// 通道数目 1～16 之间

    ADC_Init(ADC1, &ADC_InitStructure);

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
uint16_t AD_GetValue(void){
    // 软件触发 函数
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 因为 转换需要一段时间 所以 等待一下
    // 等待的方式 是获取 标志位 ; ADC_FLAG_EOC  规则组转换完成 标志位； 为 0 时 转换未完成 ; 为 1时 转换完成 ；
    // 具体的等待时间 时 采样周期 设置的55.5 +  转换 周期是固定的12.5  =  68 个周期
    // 72MHZ  下 6分频  12MHZ ； 12MHZ 进行 68 个周期 ；  1/12MHZ * 68  = 5.6us
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);// EOC 由软件清除 或 ADC_DR 时 清除； ADC_DR 数据寄存器； 一般 EOC标志位 置1 ，我们就回来读取 数据； 这一位 可以在读取数据寄存器 后自动清零；不需要再手动清除了；

    // 最终取结果
    return ADC_GetConversionValue(ADC1);// 读取 ADCX_DR 寄存器，因为读取 ADCX_DR寄存器 会自动清除 EOC； 所以 这时候就不需要 手动清除标志位了


}







