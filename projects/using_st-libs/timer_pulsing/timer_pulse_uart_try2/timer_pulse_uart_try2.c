//send out a pulse and then compture it with another timer.
//display the captured pulse length in ms on the uart
//send pulse 

#include <stm32f10x.h>
//#include <stm32f10x_rcc.h>
//#include <stm32f10x_gpio.h>
//#include <stm32f10x_tim.h>
//#include <stm32f10x_spi.h>
//#include "seven_segs.h"
#include <stm32f10x_usart.h>
#include <stdio.h>
#include "manual_uart.h"

void Delay(uint32_t nTime);

void init_timers(void);

void send_pulse_tim16(uint8_t pulse_ms);

void get_pulse_ms_tim4(void);

volatile uint16_t pulse_ms_tim4;
volatile uint16_t pulse_store;

int main(void)
{
    //GPIO_InitTypeDef	GPIO_InitStructure;

    // Enable Peripheral Clocks

    /*(1)*/

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    //Oh THIS CLOCK!
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure Pins
    /*(2)*/
    
    init_timers();
    //init_SPI1();
    //init_digit_pins();
    
    
    // Configure SysTick Timer
    /*(3)*/
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1);

    uart_open(USART1, 9600, USART_FLAG_TXE);

    
    
    while (1)	{
        int i;
        //enable TIM16
    //TIM16->CR1 |= ( TIM_CR1_CEN );//enable TIM16
    
        send_pulse_tim16(250);
        TIM16->CR1 |= ( TIM_CR1_CEN );//enable TIM16
        //Delay(110);
        while((TIM16->SR & TIM_SR_CC1IF) == 0); //wait till done
        
        pulse_store = TIM4->CCR2;
        get_pulse_ms_tim4();
        //uart_print_string_int(pulse_ms_tim4,USART1);
        //uart_putc('\n',USART1);
        //disable TIM16
    TIM16->CR1 &= ~( TIM_CR1_CEN );
        //Delay(1000);
    }   
}

// Timer code
/*(5)*/

void init_timers(void) {
    //GPIO_InitTypeDef	GPIO_InitStructure;
    
    //setup clock for GPIOB with alternate function IO allowed
    //then also enable the  TIM15 clock
    RCC->APB2ENR |= (   RCC_APB2ENR_IOPBEN  |
                        RCC_APB2ENR_AFIOEN  |
                        RCC_APB2ENR_TIM15EN );
                        
    //timer 15 remap?
    //
    
    //setup GPIOA
    RCC->APB2ENR |= (   RCC_APB2ENR_IOPAEN  );
    
    //enable TIM4 clock!!!!
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    
    
    //setup pins 
    
    //PB6-7 input floating, 
    GPIOB->CRL |= ( GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0 );
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1 |
                    GPIO_CRL_MODE6  | GPIO_CRL_MODE7  );
                    
                    
    //PB8 output push pull, alternate function, 2MHz
    GPIOB->CRH |= ( GPIO_CRH_CNF8_1 |
                    GPIO_CRH_MODE8_1);
    GPIOB->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_MODE8_0);
                    
    //PA2 TIM15_CH1
    //PUSH-PULL, AFIO, 2MHz
    GPIOA->CRL |= ( GPIO_CRL_CNF2_1 |
                    GPIO_CRL_MODE2_1);
    GPIOA->CRL &= ~(GPIO_CRL_CNF2_0 |
                    GPIO_CRL_MODE2_0);
    
    
    //setup TIM15

    //timer 15 clock 24,000,000/1,000 = 24,000 so prescaler 24,000
    //to get 1,000Hz timer clock
    TIM15->PSC = (uint16_t)(23999); //23999 + 1 = 24000
    TIM15->ARR = (uint16_t)(255); //0 to 255 ms
    //one pulse mode
    TIM15->CR1 |= ( TIM_CR1_OPM );
    //setup PWM mode two, OCIM 111, OC1 is high when CNT < CCR1
    TIM15->CCMR1 |= ( TIM_CCMR1_OC1M );
    //enable CCR1 output OC1
    TIM15->CCER |= ( TIM_CCER_CC1E );
    
    //enable TIM15
    TIM15->CR1 |= ( TIM_CR1_CEN );
    
    
    //setup TIM4 stuff
    //using PWM input mode from RM0041 page 292
    
    TIM4->PSC = (uint16_t)(23999); //to get 1,000Hz timer clock
    
    //setup CCR1 and CCR2 to go to TI1
    TIM4->CCMR1 |= (TIM_CCMR1_CC1S_0 |
                    TIM_CCMR1_CC2S_1 );
                    
    //setup CCR2 to activate on falling edge
    TIM4->CCER |= ( TIM_CCER_CC2P   );
    
    //setup the timer input triggering for TI1
    TIM4->SMCR |= ( TIM_SMCR_TS_2 | TIM_SMCR_TS_0 );
    TIM4->SMCR &= ~(TIM_SMCR_TS_1 );
    
    //slave mode selection to reset mode
    TIM4->SMCR |= ( TIM_SMCR_SMS_2 );
    TIM4->SMCR &= ~(TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);
    
    //enable the capture inputs
    TIM4->CCER |= ( TIM_CCER_CC1E | TIM_CCER_CC2E );
    
    //enable TIM4
    TIM4->CR1 |= (  TIM_CR1_CEN );
}

void get_pulse_ms_tim4(void){
    
    int difference = (TIM4->CCR2) - (TIM4->CCR1);
    
    if(difference > 0){
        pulse_ms_tim4 = difference;
    }
}

void send_pulse_tim16(uint8_t pulse_ms){
    TIM16->CCR1 = (uint16_t)(pulse_ms);
}

static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime) {
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void SysTick_Handler(void) {
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

