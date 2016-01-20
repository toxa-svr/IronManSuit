// ****************************************************************************
//  Board support package
// ****************************************************************************
//
// Global include for HAL headers (hardware access layer)
//
// ****************************************************************************
#ifndef BSP_HAL_H
#define BSP_HAL_H

#include "bsp.h"

#ifdef BOARD_IRONMAN_SUIT
	#include "hal/hal_gpio.h"
	#include "hal/hal_timer.h"
	#include "hal/hal_sleep.h" 
    #include "hal/hal_adc.h"
    #include "hal/hal_extint.h" 
    #include "hal/hal_uart.h" 
#endif


#endif // BSP_HAL_H
