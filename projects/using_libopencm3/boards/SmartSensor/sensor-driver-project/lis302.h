//library for using the lis302 accelerometer

#ifndef LIS302_H
#define LIS302_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include "my_i2c.h"


void init_lis302(void);

int filter_accel_val(int val);

#endif
