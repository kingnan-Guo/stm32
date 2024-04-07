#include "retarget.h"
 
/* 定义USART端口，用于注册重定向的串口(也可以用UART，根据实际情况来改写) */
static USART_TypeDef *sg_retargetUsart;
 
/**
 * @brief 注册重定向串口
 * @param usartx 需要重定向输入输出的串口
 */
void RetargetInit(USART_TypeDef *usartx) {
    /* 注册串口 */
    sg_retargetUsart = usartx;
     
    /* Disable I/O buffering for STDOUT stream, so that
     * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
    /* Disable I/O buffering for STDIN stream, so that
     * chars are received in as soon as they are scanned. */
    setvbuf(stdin, NULL, _IONBF, 0);
     
}
 
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE {
    /* 发送一个字节数据到串口 */
    /* 很简单，直接调用库函数中的 串口发送数据函数 */
    USART_SendData(sg_retargetUsart, (uint8_t) ch);
     
    /*等待发送完毕*/
    while (USART_GetFlagStatus(sg_retargetUsart, USART_FLAG_TXE) == RESET);
    /* 返回发送的字符 */
    return ch;
     
}
 
/**
  * @brief  Retargets the C library scanf, getchar function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE {
     
    /* 很简单，直接调用库函数中的接收 */
    /* 等待串口输入数据 */
    while (USART_GetFlagStatus(sg_retargetUsart, USART_FLAG_RXNE) == RESET);
    /* 直接返回接收到的字符 */
    return (int) USART_ReceiveData(sg_retargetUsart);
     
}