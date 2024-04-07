/**
* 避免使用微库的重定向
 *
 * 
*/

//- 在独立应用程序中，不太可能支持半主机操作。 因此，必须确保您的应用程序中没有链接C库半主机函数。
//- 为确保没有从C库链接使用半主机的函数， 必须导入符号 __use_no_semihosting 。
//- 可在您工程的任何 C 或汇编语言源文件中执行此操作，如下所示：
//- 在 C 模块中，使用 #pragma 指令：#pragma import(__use_no_semihosting)
//- 在汇编语言模块中，使用 IMPORT 指令：IMPORT __use_no_semihosting
//- 如果仍然链接了使用半主机的函数，则链接器会报告错误。
//为什么要禁用半主机模式？
//- 在嵌入式的编程中避免不了使用printf、fopen、fclose等函数，但是因为嵌入式的程序中并没有对这些函数的底层实现，使得设备运行时会进入软件中断BAEB处，这时就需要__use_no_semihosting这 个声明，使程序遇到这些文件操作函数时不停在此中断处。


#include <stdio.h>


/**
 * @brief 加入以下代码,支持 printf 函数,而不需要选择 use MicroLIB
 */
#if 1
/* 告知连接器不从C库链接使用半主机的函数 */
#pragma import(__use_no_semihosting)
/** 标准库需要的支持函数 */
struct __FILE {
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef ’ d in stdio.h. */
FILE __stdout;
/** 定义_sys_exit()以避免使用半主机模式 */
int _sys_exit(int x) {
    x = x;
}
/** 重定义 fputc 函数 */
int fputc(int ch, FILE *f) {
//    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
////    USART_SendData(USART1, (uint8_t) ch);
//    return ch;
}
/** 重定义 fgetc 函数 */
/* 这部分是我自己加的，原子本来没有 */
int fgetc(FILE *f) {
//    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//    return (int) USART_ReceiveData(USART1);
}
#endif