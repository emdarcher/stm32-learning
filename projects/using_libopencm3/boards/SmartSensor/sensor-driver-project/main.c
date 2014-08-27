//main code

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

#include "my_i2c.h"
#include "lis302.h"

#include "main.h"

volatile int8_t x_value;
volatile int8_t y_value;
volatile int8_t z_value;


//the main function 
void main(void)
{
    //main area
    
    //init system clock stuff
    init_system_clock();
    
    //init the stuff
    init_things();
    
    //int8_t x_value;
    
    //infinite loop
    while(1){
        
        x_value = lis302_get_axis(LIS302_OUTX);
        y_value = lis302_get_axis(LIS302_OUTY);
        z_value = lis302_get_axis(LIS302_OUTZ);
    }
    
}
//system clock init things
void init_system_clock(void){

    //will run at 24MHz by using pll on the internal 8Mhz HSI clock
    rcc_clock_setup_in_hsi_out_24mhz();

}
//init other things here
void init_things(void){
    //put various init function calls and things in here
    
    init_GPIO();
    
    //init_I2C1();
    //init the sensor
    init_lis302();
    
}
//init the GPIO here
void init_GPIO(void){

    //enable GPIOB clock
    //rcc_periph_clock_enable(RCC_GPIOB);
    
    
    
}
