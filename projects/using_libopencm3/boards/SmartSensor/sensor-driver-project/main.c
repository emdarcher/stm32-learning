//main code

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>


#include "main.h"



void main(void)
{
    //main area
    
    //init the stuff
    init_things();
    
    
    //infinite loop
    while(1){
        
        
        
    }
    
}

void init_things(void){
    //put various init function calls and things in here
    
    
}

void init_GPIO(void){

    //enable GPIOB clock
    rcc_periph_clock_enable(RCC_GPIOB);
    
    
    
}
