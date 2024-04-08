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
    /**
     * setvbuf
     *       int setvbuf(FILE *stream, char *buffer, int mode, size_t size) 定义流 stream 应如何缓冲；
     *          stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了一个打开的流。
     *          buffer -- 这是分配给用户的缓冲。如果设置为 NULL，该函数会自动分配一个指定大小的缓冲。
     *          mode -- 这指定了文件缓冲的模式：
     *          size --这是缓冲的大小，以字节为单位
     *
     *          如果成功，则该函数返回 0，否则返回非零值。
     *
     *   1、在main函数开始的时候，执行如下代码，设置buffer缓存为0，这样一有数据就发送，不然会等到缓存满或有回车换行符才发送。如果没有这句，你的printf又没\n，log就会打不出来。
     *   2、建议写在main的初始化最前面，或者初始化串口之前，写入这段代码 ；还有另一种方式就是再打印后，跟一行代码fflush(stdout);，这样会强制刷新缓存，就可以正常输出了，但每次都这么操作比较麻烦，所以不推荐。
     *
     *
     *
     */
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