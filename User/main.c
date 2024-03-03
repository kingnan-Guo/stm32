#include "stm32f10x.h" 

int main(void) {
	while(1) {
		// project begin

        // gpioc 的外设时钟配置完成
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        // 配置端口模式
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;// 端口
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// 模式 通用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;// 速度

        GPIO_Init(GPIOC, &GPIO_InitStructure);
        // 高电平设置
//        GPIO_SetBits(GPIOC, GPIO_Pin_13);

        // 关闭端口
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
}
