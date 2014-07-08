#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "manual_uart.h"
#include <stdio.h>
#include "xprintf.h"
// define buffers here.

int uart_open (USART_TypeDef* USARTx, uint32_t baud, uint32_t flags)
{
  USART_InitTypeDef USART_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructureTx; 
  GPIO_InitTypeDef GPIO_InitStructureRx;

  assert_param(IS_USART_123_PERIPH(USARTx));
	
  if (USARTx == USART1) {

    // Turn on clocks

    /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  |
			   RCC_APB2Periph_AFIO,
			   ENABLE);
		*/
		//manual
		RCC->APB2ENR |= ( RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN );

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		//manual		
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // Configure TX pin
    /*
    GPIO_InitStructureTx.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructureTx.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructureTx.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructureTx);
		*/
		//manual
		GPIOA->CRH |= ( GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9 );
																		//GPIO_CRH_MODE9 does GPIO_CRH_MODE9_1 and 0.	
		
		GPIOA->CRH &= ~GPIO_CRH_CNF9_0; //just make sure

    // Configure RX pin
		/*
    GPIO_InitStructureRx.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructureRx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructureRx);
		*/
		//manual
		GPIOA->CRH |= ( GPIO_CRH_CNF10_0 );
		
		GPIOA->CRH &= ~( GPIO_CRH_MODE10 | GPIO_CRH_CNF10_1 ); //make sure disabled so it is an input and floating		

    // Configure the UART
		/*
    USART_StructInit(&USART_InitStructure); 
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_Mode  = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1,&USART_InitStructure); 
    USART_Cmd(USART1, ENABLE); 
		*/
		
		USART1->BRR = SystemCoreClock/baud;
		USART1->CR1 |= ( USART_CR1_RE | USART_CR1_TE ); //enable rx and tx
		USART1->CR1 |= USART_CR1_UE; //enable uart
		
    return 0;
  } 
}


int uart_close(USART_TypeDef* USARTx)
{
    assert_param(IS_USART_123_PERIPH(USARTx));
}

int uart_putc(int c, USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
  USARTx->DR =  (c & 0xff);
  return 0;
}

int uart_putc_int(int num, USART_TypeDef* USARTx)
{
    assert_param(IS_USART_123_PERIPH(USARTx));
    
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    USARTx->DR =  (((char)(((int)'0')+num)) & 0xff);
    return 0;
} 

int uart_print_string(char pstring[],USART_TypeDef* USARTx){

                        uint16_t r;
                        for (r = 0; r < sizeof(pstring); r++){
                        uart_putc(pstring[r], USARTx);
                        }
                        uart_putc('\n',USARTx);
}

int uart_print_string_int(int num,USART_TypeDef* USARTx){
        char buff[15];
        char pstring_int[16];  
        //xsprintf(buff,"%d",num);
        xsprintf(pstring_int,"%d",num);
        
        //char pstring_int[16] = buff;
        //pstring_int[sizeof(pstring_int)-1] = '\n';
        //itoa(num,pstring_int,10);
        uart_print_string(pstring_int,USARTx);
                        /*uint16_t r;
                        for (r = 0; r < sizeof(pstring); r++){
                        uart_putc(msg_out[i], USARTx);
                        }*/
}

int uart_getc (USART_TypeDef* USARTx)
{
  assert_param(IS_USART_123_PERIPH(USARTx));

  while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
  return  USARTx->DR & 0xff;
}

