#ifndef _MANUAL_UART_H
#define _MANUAL_UART_H


#define QUEUE_SIZE 8
#define HIGH_WATER (QUEUE_SIZE - 6)


int uart_open(uint8_t uart, uint32_t baud, uint32_t flags);
int uart_close(uint8_t uart);
int uart_putc(int c, USART_TypeDef* USARTx);
int uart_getc(USART_TypeDef* USARTx);

#endif
