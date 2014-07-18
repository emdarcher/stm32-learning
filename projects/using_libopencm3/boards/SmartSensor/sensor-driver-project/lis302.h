//library for using the lis302 accelerometer

#ifndef LIS302_H
#define LIS302_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include "my_i2c.h"

#define LIS302_ADDRESS 0b0011101 //address of accelerometer with SDO high


void init_lis302(void);
void init_Aint_pins(void);

int filter_accel_val(int val);

#endif
