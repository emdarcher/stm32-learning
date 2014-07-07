//send out a pulse and then compture it with another timer.
//display the captured pulse length in ms on the 7 segment displays.
//send pulse on PB8 capture on PB9 with timer 16 and 17, respectively.

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include "seven_segs.h"

void Delay(uint32_t nTime);

void init_timers(void);

int main(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;

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
        
        for(i = 0; i < 999; i++){
            TIM_SetCompare2(TIM2, i);
            Delay(2);
        }
        for(i = 999; i > 0; i--){
            TIM_SetCompare2(TIM2, i);
            Delay(2);
        }
        
    }
}

// Timer code
/*(5)*/

void init_timers(void) {
    GPIO_InitTypeDef	GPIO_InitStructure;
    
    /*
    //setup GPIO for PA1, the output of TIM2 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
    TIM_OCInitTypeDef TIM_OCInitStructure ;
    // enable timer clock
    RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM2 , ENABLE );
    // configure timer
    // PWM frequency = 100 hz with 24 ,000 ,000 hz system clock
    //  24 ,000 ,000/240 = 100 ,000
    // 100 ,000/1000 = 100
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure );
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock /100000 - 1; // 0..239
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 0..999
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
    TIM_TimeBaseInit (TIM2, &TIM_TimeBaseStructure );
    // PWM1 Mode configuration : Channel2
    //Edge - aligned ; not single pulse mode
    TIM_OCStructInit (&TIM_OCInitStructure );
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1 ;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable ;
    TIM_OC2Init (TIM2 , &TIM_OCInitStructure );
    // Enable Timer
    TIM_Cmd (TIM2 , ENABLE );
    */
    
    
    //setup clock for GPIOB with alternate function IO allowed
    //then also enable the TIM17 and TIM16 clocks
    RCC->APB2ENR |= (   RCC_APB2ENR_IOPBEN  |
                        RCC_APB2ENR_AFIOEN  |
                        RCC_APB2ENR_TIM17EN |
                        RCC_APB2ENR_TIM16EN );
    
    
    //setup PB9 and PB8
    //PB9 input floating, 
    GPIOB->CRH |= ( GPIO_CRH_CNF9_0 );
    GPIOB->CRH &= ~(GPIO_CRH_CNF9_1 |
                    GPIO_CRH_MODE9  ):
    //PB8 output push pull, alternate function, 2MHz
    GPIOB->CRH |= ( GPIO_CRH_CNF8_1 |
                    GPIO_CRH_MODE8_1);
    GPIOB->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_MODE8_0);
    
    
    //setup TIM16

    //timer 16 clock 24,000,000/1,000 = 24,000 so prescaler 24,000
    //to get 1,000Hz timer clock
    TIM16->PSC = (uint16_t)(23999); //23999 + 1 = 24000
    TIM16->ARR = (uint16_t)(255); //0 to 255 ms
    //one pulse mode
    TIM16->CR1 |= ( TIM_CR1_OPM );
    //setup PWM mode two, OCIM 111, OC1 is high when CNT < CCR1
    TIM16->CCMR1 |= ( TIM_CCMR1_OC1M );
    //enable CCR1 output OC1
    TIM16->CCER |= ( TIM_CCER_CC1E );
    
    //enable TIM16
    TIM16->CR1 |= ( TIM_CR1_CEN );
    
    
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

