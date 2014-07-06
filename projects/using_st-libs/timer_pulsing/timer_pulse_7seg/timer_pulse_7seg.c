//pwm-stuff for LED

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include "seven_segs.h"

void init_SPI1(void);
void init_digit_pins(void);
void write_SPI1(uint8_t out_byte);
void msg_error(void);

void write_number(int16_t number);
void write_digit(int8_t num, uint8_t dig);


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

void init_digit_pins(void){
    
    //config rcc clocks and enable GPIO C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    //setup GPIO PC0-PC3 for output push pull max speed 50MHz (too fast?)
    GPIOC->CRL |= ( GPIO_CRL_MODE0  |
                    GPIO_CRL_MODE1  |
                    GPIO_CRL_MODE2  |
                    GPIO_CRL_MODE3  );
    GPIOC->CRL &= ~(GPIO_CRL_CNF0   |
                    GPIO_CRL_CNF1   |
                    GPIO_CRL_CNF2   |
                    GPIO_CRL_CNF3   );
                    
    
}

void init_SPI1(void){
    //configure clocks for the stuff
    
    //on APB2
    //enable SPI1, GPIO A, Alternate function IO.
    RCC->APB2ENR |= (   RCC_APB2ENR_SPI1EN | 
                        RCC_APB2ENR_IOPAEN | 
                        RCC_APB2ENR_AFIOEN );
    
    
    //configure GPIO Pins used for SPI1
    
    //SCK pin PA5
    //setup output 50MHz, alternate function OUTPUT Push-Pull
    GPIOA->CRL |= ( GPIO_CRL_MODE5 | GPIO_CRL_CNF5_1 );
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    //MISO not used right now PA6
    //....
    GPIOA->CRL |= GPIO_CRL_CNF6_1;
    GPIOA->CRL &= ~( GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6 );
    
    
    //MOSI pin PA7
    //same kind of setup as the SCK pin
    GPIOA->CRL |= ( GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1 );
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
    
    //NSS/SS/CS pin PA4
    //general purpose output push pull 50MHz 
    GPIOA->CRL |= (GPIO_CRL_MODE4 );
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    
    //test CS on PA3
    GPIOA->CRL |= (GPIO_CRL_MODE3 );
    GPIOA->CRL &= ~GPIO_CRL_CNF3;
    
    //SPI1 setup stuff
    //setup 2 line unidirecional data mode
    //setup for 8 bit mode
    //setup software slave management for now
    //setup MSB first
    //CPOL = 0
    //CPHA = 0
    SPI1->CR1 &= ~( SPI_CR1_BIDIMODE | 
                    SPI_CR1_DFF | 
                    //SPI_CR1_SSM | 
                    SPI_CR1_LSBFIRST |
                    SPI_CR1_CPOL | 
                    SPI_CR1_CPHA |
                    SPI_CR1_RXONLY);
    
    //baud rate CLK/div64 slower at 375,000Hz
    //master mode
    SPI1->CR1 |= ( SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_MSTR );
    SPI1->CR1 &= ~SPI_CR1_BR_1;
    //SPI1->CR2 |= SPI_CR2_SSOE;
    SPI1->CR2 &= ~SPI_CR2_SSOE;
    //CRC polynomial ?
    //SPI1->CRCPR = 4;
    
    SPI1->CR1 |= ( SPI_CR1_SSM | SPI_CR1_SSI );
    
    //disable remap
    AFIO->MAPR &= ~AFIO_MAPR_SPI1_REMAP;
    
    //enable SP1
    SPI1->CR1 |= SPI_CR1_SPE;
}

void write_SPI1(uint8_t out_byte){
    //SPI1->CR1 &= ~SPI_CR1_SSI;
    SPI1->DR = out_byte;
    //set_NSS();
    //SPI_I2S_SendData(SPI1, out_byte);
    //spiReadWrite (SPI2 , rxbuf , SPI_out_byte , 4, SPI_SLOW );
    while( !(SPI1->SR & SPI_SR_TXE) ); // wait until transmit complete
        //set_NSS();
    while( SPI1->SR & SPI_SR_BSY ); // wait until SPI is not busy anymore
    //set_NSS();
    //SPI1->CR1 |= SPI_CR1_SSI;
}

void write_digit(int8_t num, uint8_t dig){
	uint8_t k;
    
    GPIOA->BRR |= (1<<3); //put SS/CS low
    GPIOA->BRR |= (1<<4);
	if((num < 10) && (num >= 0)){
    SPI_out_byte = number_seg_bytes[num];
    } else {
    SPI_out_byte = number_seg_bytes[10];
    }
	write_SPI1(SPI_out_byte);
    //SPI1->DR = SPI_out_byte;
	for( k = 0; k < num_digits; k++){
		if ( k == dig ){
			//digits_out |= digit_bits[k];
            GPIOC->BSRR |= digit_bits[k];
		} else {
			//digits_out &= ~(digit_bits[k]);
            GPIOC->BRR |= (digit_bits[k]);
		}
	}
    GPIOA->BSRR |= (1<<4);
	GPIOA->BSRR |= (1<<3); //put SS/CS high again to latch shift register
	//flip_latch();
    Delay(25);
}

void msg_error(void){
	write_digit(10, 0);
}

void write_number(int16_t number){
		uint8_t h;
		int16_t format_num = number;
		//check if number is too big ot not
		if ((number < 10000) && (number >= 0)){
			//formats number based on digits to correct digits on display
			for(h=0;h < num_digits;h++){
				write_digit(format_num % 10, h);
				format_num /= 10;
			}         
		} else {
			msg_error();
		}
        //while( SPI1->SR & SPI_SR_BSY ); // wait until SPI is not busy anymore
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{

    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif

