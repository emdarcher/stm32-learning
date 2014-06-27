//blinking lights tutorial/walkthrough in book

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void Delay(uint32_t nTime);

int main(void)
{
//		GPIO_InitTypeDef	GPIO_InitStructure;

	// Enable Peripheral Clocks
	
	/*(1)*/

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	//manual
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	//Oh THIS CLOCK!
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//manual
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// Configure Pins
	/*(2)*/
/*		
GPIO_StructInit(&GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 + GPIO_Pin_8; //adding both GPIOs!
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
	*/
	//manually
	
	//GPIOC_CRH |= (1<<1); 	
  //GPIOC->CRH |= (1<<1);
	GPIOC->CRH = GPIO_CRH_MODE8_1;	
	
	//GPIO_Init(GPIOC,	&GPIO_InitStructure);
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
	//	GPIOA_CRL |= (1<<2);
	//GPIOA->CRL |= (1<<2);
	GPIOA->CRL	|= GPIO_CRL_CNF0_0;	

	// Configure SysTick Timer
	/*(3)*/
	//if (SysTick_Config(SystemCoreClock / 1000))
	//		while (1);

	while (1)	{
		//static int ledval = 0;
		
		uint16_t input_port_vals = GPIOA->IDR;		
			
		if ((input_port_vals & (1<<0)) == (1<<0)){
			GPIOC->BSRR |= (1<<8);
		}else{
			GPIOC->BRR |= (1<<8);
		}

		/*
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1){
			//ledval=1;
			//GPIO_SetBits sets the desired bits
			GPIO_SetBits(GPIOC, ( GPIO_Pin_9 | GPIO_Pin_8 ));
		}else{
		// toggle led
		////(4)/
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, (ledval) ? Bit_RESET : Bit_SET);
		ledval = 1 - ledval;

		Delay(250);		// wait 250ms
		}*/
	}
}

// Timer code
/*(5)*/

static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}
/*
void SysTick_Handler(void){
	if (TimingDelay != 0x00)
		TimingDelay--;
}*/

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{

	/* Infinite loop */
	/* Use GDB to find out why we're here */
	while (1);
}
#endif

