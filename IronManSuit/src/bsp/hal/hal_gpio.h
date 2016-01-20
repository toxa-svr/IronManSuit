// ****************************************************************************
// Hardware access layer for all ATmega MCU
// ****************************************************************************
//
// GPIO control macros
//
// ****************************************************************************
#ifndef HAL_GPIO_H
#define HAL_GPIO_H


#include <avr/io.h>

// ****************************************************************************
// Macros for common GPIO control
// port - to set port value in output mode or set pulling in input mode (0 - no pull-up, 1 - with pull-up)
// ddr  - data direction port (0 - input, 1 - output)
// pin  - port input (to get port value in both input and output modes)
// ****************************************************************************
#define HAL_GPIO_INIT_OUT_UP(port,ddr,bit)  { port |= (1<<bit); ddr  |= (1<<bit); } // init as output and set high      
#define HAL_GPIO_INIT_OUT_DN(port,ddr,bit)  { port &=~(1<<bit); ddr  |= (1<<bit); } // init as output and set low
#define HAL_GPIO_INIT_INP_PU(port,ddr,bit)  { ddr  &=~(1<<bit); port |= (1<<bit); } // init as input with pull-up
#define HAL_GPIO_INIT_INP_ZZ(port,ddr,bit)  { ddr  &=~(1<<bit); port &=~(1<<bit); } // init as input not pulled
#define HAL_GPIO_OUT_UP(port,bit)           { port |= (1<<bit); }
#define HAL_GPIO_OUT_DN(port,bit)           { port &=~(1<<bit); }
#define HAL_GPIO_OUT_TOGGLE(port,bit)       { port ^= (1<<bit); }
#define HAL_GPIO_IS_UP(pin,bit)             (pin & (1<<bit))


#endif // HAL_GPIO_H
