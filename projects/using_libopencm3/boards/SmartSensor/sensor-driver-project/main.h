//main header

#ifndef MAIN_H
#define MAIN_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>


#include "my_i2c.h"
#include "lis302.h"

//prototypes
void init_things(void);
void init_GPIO(void);
void init_system_clock(void);

#endif
