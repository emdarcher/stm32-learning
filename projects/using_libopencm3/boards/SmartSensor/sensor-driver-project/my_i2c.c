

void init_I2C1(void){
    
    //setup gpio
    init_I2C1_GPIO();
    
    //enable clock
    rcc_periph_clock_enable(RCC_I2C1);
    
    //setup I2C1 peripheral
    
    
    
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
