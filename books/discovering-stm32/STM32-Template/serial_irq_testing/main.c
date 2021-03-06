//uses uart to send Hello World

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "manual_uart.h"
#include <misc.h>


void Delay(uint32_t nTime);


int main(void){
  
  	GPIO_InitTypeDef        GPIO_InitStructure;

		if (SysTick_Config(SystemCoreClock / 1000))
                        while (1);
		
		uart_open(1, 9600, USART_FLAG_TXE);
		while(1){
			

			char msg_out[] = "Hello World\n\r";				
			uint16_t i;
			for (i = 0; i < sizeof(msg_out); i++){
			uart_write(msg_out[i], USART1);
			}
			//uart_putc('H', USART1);
			
			Delay(250);
		}
}
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime){
        TimingDelay = nTime;
        while(TimingDelay != 0);
}

void SysTick_Handler(void){
        if (TimingDelay != 0x00)
                TimingDelay--;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{

        /* Infinite loop */
        /* Use GDB to find out why we're here */
        while (1);
}
#endif
