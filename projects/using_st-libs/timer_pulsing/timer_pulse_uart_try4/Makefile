TEMPLATEROOT = ../../

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) timer_pulse_uart_try4.o
#extra stuff
OBJS+=	stm32f10x_gpio.o 
OBJS+= stm32f10x_rcc.o 
#OBJS+= stm32f10x_tim.o 
#OBJS+= stm32f10x_spi.o 
OBJS+= stm32f10x_usart.o
#OBJS+= seven_segs.o 
OBJS+= manual_uart.o
OBJS+= xprintf.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common


