//library for using the lis302 accelerometer

#ifndef LIS302_H
#define LIS302_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include "my_i2c.h"

#define LIS302_ADDRESS 0b00011101 //address of accelerometer with SDO high

//registers
#define LIS302_CTRL_REG1    0x20
#define LIS302_CTRL_REG2    0x21
#define LIS302_CTRL_REG3    0x22
#define LIS302_STATUS_REG   0x27
#define LIS302_OUTX         0x29
#define LIS302_OUTY         0x2b
#define LIS302_OUTZ         0x2d



void init_lis302(void);
void init_Aint_pins(void);

void lis302_write(uint8_t reg, uint8_t byte);//add params and stuff
void lis302_read(void);//add params later

uint8_t lis302_get_axis(uint8_t axis_reg);
//int8_t lis302_get_y(void);

int filter_accel_val(int val);

#endif
