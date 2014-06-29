#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include "manual_spi.h"

static const uint16_t speeds[] = {
  [SPI_SLOW] = SPI_BaudRatePrescaler_64,
  [SPI_MEDIUM] = SPI_BaudRatePrescaler_8,
  [SPI_FAST] = SPI_BaudRatePrescaler_2};

void spiInit(SPI_TypeDef *SPIx)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_StructInit(&GPIO_InitStructure);
  SPI_StructInit(&SPI_InitStructure);

  if (SPIx == SPI1) {  
    /* Enable clocks, configure pins */
    //...  You need to enable clocks and pins !
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
   }  else  {  //  other SPI devices --
      return;
  }

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//SPI1->CR1 &= ~SPI_CR1_RXONLY;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = speeds[SPI_SLOW];
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPIx, &SPI_InitStructure);

  SPI_Cmd(SPIx, ENABLE);   
}

int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf, 
         const uint8_t *tbuf, int cnt, enum spiSpeed speed)
{
  int i;

  SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | 
               speeds[speed];

  for (i = 0; i < cnt; i++){
    if (tbuf) {
      SPI_I2S_SendData(SPIx, *tbuf++);
    } else {
      SPI_I2S_SendData(SPIx, 0xff);
    }
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
    if (rbuf) {
      *rbuf++ = SPI_I2S_ReceiveData(SPIx);
    } else {
      SPI_I2S_ReceiveData(SPIx);
    }
  }
  return i;
}

int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, 
     const uint16_t *tbuf, int cnt, enum spiSpeed speed)
{
  // unimplemented !
}



