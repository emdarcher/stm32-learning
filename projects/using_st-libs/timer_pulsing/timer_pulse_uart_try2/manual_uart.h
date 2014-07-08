#ifndef _MANUAL_UART_H
#define _MANUAL_UART_H

int uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags);
int uart_close(USART_TypeDef* USARTx);
int uart_putc(int c, USART_TypeDef* USARTx);
int uart_getc(USART_TypeDef* USARTx);
int uart_putc_int(int num, USART_TypeDef* USARTx);
int uart_print_string(char pstring[],USART_TypeDef* USARTx);
int uart_print_string_int(int num,USART_TypeDef* USARTx);


#endif
