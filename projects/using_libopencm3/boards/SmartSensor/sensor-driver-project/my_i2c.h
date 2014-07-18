//my i2c commands library

#ifndef MY_I2C_H
#define MY_I2C_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

void init_I2C1(void);
void init_I2C1_GPIO(void);

void I2C1_write(void); //add some input args for these later
void I2C1_read(void);

#endif
