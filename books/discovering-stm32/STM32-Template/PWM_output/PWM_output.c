//blinking lights tutorial/walkthrough in book

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>


void Delay(uint32_t nTime);

void init_timers(void);

int main(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;

    // Enable Peripheral Clocks

    /*(1)*/

    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    //Oh THIS CLOCK!
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // Configure Pins
    /*(2)*/
    /*
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 + GPIO_Pin_8; //adding both GPIOs!
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,	&GPIO_InitStructure);
    */
    //
    /*
    //button config here I think with a new GPIO_InitStructure thingy
    GPIO_StructInit(&GPIO_InitStructure);
    //make sure to do clock init first?
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    */
    
    
    init_timers();

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

