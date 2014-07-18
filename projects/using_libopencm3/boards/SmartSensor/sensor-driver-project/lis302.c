//library for lis302

#include "lis302.h"


void init_lis302(void){
    
    //init the A-int pins
    init_Aint_pins();
    
    //init I2C1
    init_I2C1();
    
}


void init_Aint_pins(void){
    
    //enable GPIOB clock
    rcc_periph_clock_enable(RCC_GPIOB);
    
    //setup gpio for the A-int pins
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, 
                    GPIO_CNF_INPUT_FLOAT,
                    (GPIO8 | GPIO9) );
    
}
