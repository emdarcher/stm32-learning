//going to try controlling 4 7segment displays with a shift register
//controlled via SPI

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
//#include "manual_spi.h"
#include "seven_segs.h"

void init_SPI1(void);
void init_digit_pins(void);

void Delay(uint32_t nTime);

int main(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;

    // Enable Peripheral Clocks

    /*(1)*/

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // Configure Pins
    /*(2)*/

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_2MHz;
    GPIO_Init(GPIOC,	&GPIO_InitStructure);

    // Configure SysTick Timer
    /*(3)*/
    if (SysTick_Config(SystemCoreClock / 1000))
        while (1);

    while (1)	{
        static int ledval = 0;

        // toggle led
        /*(4)*/
        GPIO_WriteBit(GPIOC, GPIO_Pin_9, (ledval) ? Bit_SET : Bit_RESET);
        ledval = 1 - ledval;

        Delay(250);		// wait 250ms
    }
}

// Timer code
/*(5)*/

static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime) {
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void SysTick_Handler(void) {
    if (TimingDelay != 0x00)
        TimingDelay--;
}



void init_SPI1(void){
    //configure clocks for the stuff
    
    //on APB2
    //enable SPI1, GPIO A, Alternate function IO.
    RCC->APB2ENR |= ( SPI1EN | IOPAEN | AFIOEN );
    
    
    //configure GPIO Pins used for SPI1
    
    //SCK pin PA5
    //setup output 50MHz, alternate function OUTPUT Push-Pull
    GPIOA->CRL |= ( GPIO_CRL_MODE5 | GPIO_CRL_CNF5_1 );
    GPIOA->CRL &= ~GPIO_CRL_CNF5_0;
    //MISO not used right now
    //....
    
    //MOSI pin PA7
    //same kind of setup as the SCK pin
    GPIOA->CRL |= ( GPIO_CRL_MODE7 | GPIO_CRL_CNF7_1 );
    GPIOA->CRL &= ~GPIO_CRL_CNF7_0;
    
    //NSS/SS/CS pin PA4
    //general purpose output push pull 50MHz 
    GPIOA->CRL |= (GPIO_CRL_MODE4 );
    GPIOA->CRL &= ~GPIO_CRL_CNF4;
    
    //SPI1 setup stuff
    //setup 2 line unidirecional data mode
    //setup for 8 bit mode
    //setup software slave management for now
    //setup MSB first
    //CPOL = 0
    //CPHA = 0
    SPI1->CR1 &= ~( SPI_CR1_BIDIMODE | 
                    SPI_CR1_DFF | 
                    SPI_CR1_SSM | 
                    SPI_CR1_LSBFIRST |
                    SPI_CR1_CPOL | 
                    SPI_CR1_CPHA );
    
    //baud rate CLK/div64 slower
    //master mode
    SPI1->CR1 |= ( SPI_CR1_BR_0 | SPI_CR1_BR_2 | SPI_CR1_MSTR );
    SPI1->CR1 &= ~SPI_CR1_BR_1;
    
    //CRC polynomial ?
    SPI1->CRCPR = 7;
    
    //enable SP1
    SPI1->CR1 |= SPI_CR1_SPE;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{

    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif

