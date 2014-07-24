//library for lis302

#include "lis302.h"
#include <stdint.h>

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

uint8_t lis302_get_axis(uint8_t axis_reg){
    
    uint32_t reg32 __attribute__((unused));
    
    uint8_t axis_data;
    
    //start i2c1
    i2c_send_start(I2C1);
    
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB) 
    & (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
    //while (!((I2C1_SR1 & I2C_SR1_SB) 
    //& (I2C1_SR2 & (I2C_SR2_MSL | I2C_SR2_BUSY))));
    
    
    //send address write
    i2c_send_7bit_address(I2C1, LIS302_ADDRESS, I2C_WRITE);
    
    /* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    
    /* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);
    
    //send the command register
    i2c_send_data(I2C1, axis_reg);
    
    while (!(I2C_SR1(I2C1) & (I2C_SR1_BTF | I2C_SR1_TxE)));
    
    //send start again
    i2c_send_start(I2C1);
    
    while (!((I2C_SR1(I2C1) & I2C_SR1_SB)
		& (I2C_SR2(I2C1) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
    
    //send address read 
    i2c_send_7bit_address(I2C1, LIS302_ADDRESS, I2C_READ);
    
    /* Waiting for address is transferred. */
	while (!(I2C_SR1(I2C1) & I2C_SR1_ADDR));
    
    /* Cleaning ADDR condition sequence. */
	reg32 = I2C_SR2(I2C1);
    
    //get data in
    axis_data = i2c_get_data(I2C1);
    
    while (!(I2C_SR1(I2C1) & I2C_SR1_BTF))
    
    //send stop
    i2c_send_stop(I2C1);
    
    return axis_data;
}

