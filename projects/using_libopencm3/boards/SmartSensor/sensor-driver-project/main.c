//main code

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "my_i2c.h"
#include "lis302.h"

#include "main.h"



void main(void)
{
    //main area
    
    //will run at 24MHz by using pll on the internal 8Mhz HSI clock
    rcc_clock_setup_in_hsi_out_24mhz();
    
    //init the stuff
    init_things();
    
    
    //infinite loop
    while(1){
        
        
        
    }
    
}

void init_things(void){
    //put various init function calls and things in here
    
    init_GPIO();
    
    //init_I2C1();
    //init the sensor
    init_lis302();
    
}

void init_GPIO(void){

    //enable GPIOB clock
    //rcc_periph_clock_enable(RCC_GPIOB);
    
    
    
}
