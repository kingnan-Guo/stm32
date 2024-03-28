/**
 * 1、开启时钟、 开启GPIO 和 定时器时钟
 * 2、配置GPIO PA6 PA7 输入模式
 * 3、配置时基单元 ； 预分频器 不分频； 自动重装 65535
 * 4、 配置输入捕获单元； 只有 滤波球 极性 有用 ；其他参数没有用
 * 5、配置编码器接口模式
 * 6、 TIM_Cmd 启动 定时器
 *
 * 配置完成之后 cnt 会随着 编码器的旋转 自增自减
 *
 * 如果想要测量编码器的位置，那么直接读取CNT的值
 * 如果想测量速度和方向，那就每隔一段固定的闸门时间段，取出一次CNT 然后CNT清零
 */
#include "stm32f10x.h"
int16_t TEMP;
int16_t TEMP2;
void Encode_Init(void){

    // 开启时钟 TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /**
     * 配置GPIO
     * 因为 引脚 要使用 GPIO 所以 先初始化 GPIO
     * ITM3_CH1 对应 引脚 PA6
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 滤波器   TIM_CKD_DIV1 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//编码器状态下 没有用 ;计数器 模式 ； TIM_CounterMode_Up 向上 计数
    TIM_TimeBaseInitStructure.TIM_Period =  65536 - 1; // 周期 ARR 自动重装器的值 (有一个数的偏差 ，取值 要早65535 之间)； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1  - 1;// 0 不分频；PSC 预分频 的值 (有一个数的偏差 ，取值 要早65535 之间)；  对 72MHZ 进行7200 分频 得到10k 计数频率； 在 10k 的频率下 计数10000 就是 1s
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;// 重复计数器的值 （高级计数器才有的值）
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);// 时基单元 初始化

    /**
     * 配置  初始化 输入
     *  通道 1 盆中 上升沿 触发
     *  通道 2 配置 下降沿 触发
     */
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);// TIM_ICInitStructure 赋初始值
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;// 选择 通道 TIM_Channel_1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;// 配置输入捕获的滤波器 0x0 ～ 0xF 值越大 滤波效果 越好
    //TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//极性 ； 边沿检测 极性检查； 选择  TIM_ICPolarity_Rising 上升沿触发 还是 TIM_ICPolarity_Falling下降沿触发； 这里的上升沿代表 高低定平极性不反转； 下降沿 就是反向
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    // 配置通道 2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;// 选择 通道 TIM_Channel_2
    TIM_ICInitStructure.TIM_ICFilter = 0xF;// 配置输入捕获的滤波器 0x0 ～ 0xF 值越大 滤波效果 越好
    //TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//极性 ； 边沿检测 极性检查； 选择  TIM_ICPolarity_Rising 上升沿触发 还是 TIM_ICPolarity_Falling下降沿触发
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    /**
     * 配置编码器接口
     * ; ;TIM_EncoderMode_TI12 编码器 模式 TI1 TI2 都计数；
     * TIM_ICPolarity_Rising 配置极性   == TIM_ICInitStructure.TIM_ICPolarity 效果也是一样的； 配置的都是同一个寄存器  ；T
     * IM_EncoderInterfaceConfig 在 TIM_ICInit 的下面； 下面会把上面覆盖
     *
     * TIM_ICPolarity_Rising 改变一个 会反转极性； 改变两个 就不变了
     */
    TIM_EncoderInterfaceConfig(TIM3,  TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    //启动定时器
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * int16_t 实现负数
 * @return
 */
int16_t Encode_Get(void){
    return TIM_GetCounter(TIM3);// 获取 CNT 的值
}

/**
 * 每次 闸门时间段 获取完 CNT 后
 * CNT清零
 * @return
 */
int16_t  Encode_get_and_ClearCnt(void){
    int16_t TEMP3;
    TEMP = TIM_GetCounter(TIM3);
    // 给编码器 清零
    //TIM_SetCounter(TIM3, 0);
    TEMP3 = TEMP - TEMP2;

    TEMP2 = TEMP;
    return TEMP3;
}