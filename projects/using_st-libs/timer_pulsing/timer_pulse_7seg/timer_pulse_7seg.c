//send out a pulse and then compture it with another timer.
//display the captured pulse length in ms on the 7 segment displays.
//send pulse on PB8 capture on PB6-7 with timer 16 and 4, respectively.

#include <stm32f10x.h>
//#include <stm32f10x_rcc.h>
//#include <stm32f10x_gpio.h>
//#include <stm32f10x_tim.h>
//#include <stm32f10x_spi.h>
#include "seven_segs.h"

void Delay(uint32_t nTime);

void init_timers(void);

void send_pulse_tim16(uint8_t pulse_ms);

void get_pulse_ms_tim4(void);

uint16_t pulse_ms_tim4;
uint16_t pulse_store;
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
    init_SPI1();
    init_digit_pins();
    
    
    // Configure SysTick Timer
    /*(3)*/
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1);

    while (1)	{
        int i;
        
        //for (i = 0; i < 1000;i++){
        //    write_number(pulse_ms_tim4);
        //}
        //send_pulse_tim16(100);
        //while((TIM16->SR & TIM_SR_CC1IF) == 0); //wait till done
        //Delay(200);
        pulse_store = TIM4->CCR2;
        get_pulse_ms_tim4();
        //for (i=0;i<1000;i++){
            //write_number(pulse_ms_tim4);
            write_number(pulse_store);
        //}
    }   
}

// Timer code
/*(5)*/

void init_timers(void) {
    //GPIO_InitTypeDef	GPIO_InitStructure;
    
    //setup clock for GPIOB with alternate function IO allowed
    //then also enable the  TIM16 clock
    RCC->APB2ENR |= (   RCC_APB2ENR_IOPBEN  |
                        RCC_APB2ENR_AFIOEN  |
                        RCC_APB2ENR_TIM16EN );
    
    
    //setup pins 
    
    //PB6-7 input floating, 
    GPIOB->CRL |= ( GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0 );
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1 |
                    GPIO_CRL_MODE6  | GPIO_CRL_MODE7  );
                    
                    
    //PB8 output push pull, alternate function, 2MHz
    GPIOB->CRH |= ( GPIO_CRH_CNF8_1 |
                    //GPIO_CRH_MODE8_1|
                    GPIO_CRH_MODE8);
    //GPIOB->CRH &= ~(GPIO_CRH_CNF8_0 |
    //                GPIO_CRH_MODE8_0);
    
    
    //setup TIM16

    //timer 16 clock 24,000,000/1,000 = 24,000 so prescaler 24,000
    //to get 1,000Hz timer clock
    TIM16->PSC = (uint16_t)(23999); //23999 + 1 = 24000
    TIM16->ARR = (uint16_t)(255); //0 to 255 ms
    //one pulse mode
    //TIM16->CR1 |= ( TIM_CR1_OPM );
    //setup PWM mode two, OCIM 111, OC1 is high when CNT < CCR1
    TIM16->CCMR1 |= ( TIM_CCMR1_OC1M );
    TIM16->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    //enable CCR1 output OC1
    TIM16->CCER |= ( TIM_CCER_CC1E );
    
    
    //one pulse mode
    TIM16->CR1 |= ( TIM_CR1_OPM );
    
    //enable TIM16
    //TIM16->CR1 |= ( TIM_CR1_CEN );
    send_pulse_tim16(34);
    
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
    
    int difference = (TIM4->CCR2);
    
    if(difference > 0){
        pulse_ms_tim4 = difference;
    }
}

void send_pulse_tim16(uint8_t pulse_ms){
    TIM16->CCR1 = (uint16_t)(pulse_ms);
    TIM16->CR1 |= (TIM_CR1_CEN);
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

