//=============================================================================
// STM32VLDISCOVERY tutorial
// Lesson 6. Pusle Width Modulation (PWM)
// Copyright : Radoslaw Kwiecien
// http://en.radzio.dxp.pl
// e-mail : radek(at)dxp.pl
//=============================================================================
#include "stm32f10x.h"
//#include "antilib_gpio.h"
//=============================================================================
// Defines
//=============================================================================
#define LED_BLUE_GPIO GPIOC
#define LED_BLUE_PIN 8

#define LED_GREEN_GPIO GPIOC
#define LED_GREEN_PIN 9
//=============================================================================
// main function
//=============================================================================
int main(void)
{
RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
//#if (LED_BLUE_PIN > 7)
//  LED_BLUE_GPIO->CRH = (LED_BLUE_GPIO->CRH & CONFMASKH(LED_BLUE_PIN)) |
//    GPIOPINCONFH(LED_BLUE_PIN, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL));
//#else
//  LED_BLUE_GPIO->CRL = (LED_BLUE_GPIO->CRL & CONFMASKL(LED_BLUE_PIN)) |
//    GPIOPINCONFL(LED_BLUE_PIN, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL));
//#endif

//#if (LED_GREEN_PIN > 7)
//  LED_GREEN_GPIO->CRH = (LED_GREEN_GPIO->CRH & CONFMASKH(LED_GREEN_PIN)) |     GPIOPINCONFH(LED_GREEN_PIN, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL));
//#else
//  LED_GREEN_GPIO->CRL = (LED_GREEN_GPIO->CRL & CONFMASKL(LED_GREEN_PIN)) |     GPIOPINCONFL(LED_GREEN_PIN, GPIOCONF(GPIO_MODE_OUTPUT2MHz, GPIO_CNF_AFIO_PUSHPULL));
//#endif
 //with AFIO for TIM3
    GPIOC->CRH |= ( GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9 );
    //GPIOB->CRL |= ( GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1_1);
    
    //PC8 output push-pull 2MHz
    //GPIOC->CRH |= ( GPIO_CRH_MODE8_1 );
    //for AFIO
    GPIOC->CRH |= ( GPIO_CRH_CNF8_1 | GPIO_CRH_MODE8 );
    //GPIOB->CRL |= ( GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0_1 );
AFIO->MAPR = AFIO_MAPR_TIM3_REMAP; // Full TIM3 remap

TIM3->PSC = 239; // Set prescaler to 24 000 (PSC + 1)
TIM3->ARR = 500; // Auto reload value 500

TIM3->CCR3 = 0;   // Start PWM duty for channel 3
TIM3->CCR4 = 500; // Start PWM duty for channel 4
TIM3->CCMR2 = TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 |
              TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1; // PWM mode on channel 3 & 4

TIM3->CCER = TIM_CCER_CC4E | TIM_CCER_CC3E; // Enable compare on channel 3 & 4
TIM3->CR1  = TIM_CR1_CEN; // Enable timer
//TIM3->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
//NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt from TIM3 (NVIC level)
while (1) {}
}
//=============================================================================
// TIM3 Interrupt Handler
//=============================================================================
/*void TIM3_IRQHandler(void)
{
if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
  {
  TIM3->SR &= ~TIM_SR_UIF;  // clear UIF flag
  TIM3->CCR3 += 5;          // increase ch3 duty
  if(TIM3->CCR3 == 500)     // if maximum
    TIM3->CCR3 = 0;         // set to zero
  TIM3->CCR4 -= 5;          // decrease ch4 duty
  if(TIM3->CCR4 == 0)       // if minimum
    TIM3->CCR4 = 500;       // set to maximum
}
}*/
//=============================================================================
// End of file
//=============================================================================

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{

    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif


