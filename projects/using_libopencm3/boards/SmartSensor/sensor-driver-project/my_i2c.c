#include "my_i2c.h"

void init_I2C1(void){
    
    //setup gpio
    init_I2C1_GPIO();
    
    //enable afio clock
    rcc_periph_clock_enable(RCC_AFIO);
    
    //enable clock
    rcc_periph_clock_enable(RCC_I2C1);
    
    //setup I2C1 peripheral
    i2c_peripheral_enable(I2C1);
    //set duty cycle
    i2c_set_dutycycle(I2C1, I2C_CCR_DUTY_DIV2);
    //set own address, put to 0x00 b/c we don't really use it
    //as we are the master here!
    i2c_set_own_7bit_slave_address(I2C1, 0x00);
    //enable ACK
    i2c_enable_ack(I2C1);
    //set 7 bit mode 
    I2C1_OAR1 &= ~I2C_OAR1_ADDMODE;
    //set clock speed
    i2c_set_clock_frequency(I2C1, I2C_CR2_FREQ_8MHZ);
    i2c_set_ccr(I2C1, 0x28); //should get us a 100kHz clock
    
}

void init_I2C1_GPIO(void){
    
    //setup the gpio for i2c1
    rcc_periph_clock_enable(RCC_GPIOB);
    
    //setup GPIO pins PB7 and PB6
    //for af output open-drain
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, 
                    GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
                    (GPIO6 | GPIO7) );
    
}
