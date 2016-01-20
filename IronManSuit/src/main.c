// ****************************************************************************
// IronManSuit main
//
// Hardware initialization, main loop to process events
// ****************************************************************************

//#include <avr/io.h>
//#include <util/delay.h>
//#include <stdint.h>
//#include <string.h>
//#include <stdio.h>
#include <stdbool.h>

#include "bsp.h"
#include "bsp_trace.h"
#include "bsp_gpio.h"
#include "bsp_timers.h"
#include "bsp_sleep.h"
#include "bsp_extint.h"
#include "suitcontrol.h"



int main(void)
{
    // Disable clocks for blocks which will be never used
    POWER_ALL_DISABLE();
    POWER_UART_ENABLE();
    POWER_ADC_ENABLE();
    POWER_TIMER0_ENABLE();
    POWER_TIMER1_ENABLE();
    POWER_TIMER2_ENABLE();

    BSP_BTNS_INIT();
    BSP_LEDS_INIT();
    BSP_LEDS_OFF();
    BSP_LEDS_TEST_BLINK();

    BSP_timer_init(); 
    BSP_extint_init(0, true);
    BSP_extint_enable(0);
    BSP_uart_init();
    BSP_uart_enable();
    
    BSP_ALL_INT_ENABLE();
    BSP_TRACE("\r\n\r\nIRON MAN SUIT", 0);
    BSP_TRACE("Compiled: %s, %s", __DATE__, __TIME__);
    
    
	while (1) 
    {
         // Process software timers
         BSP_timer_process_all();

         // Process battery measurement
         processBatteryControl();
         
         // Process buttons
         checkPressed();
         processButtonEvent();
         
         // Process LEDs
         processLeds();
                 
         // Process effects 
         processEffects();
         
         // Sleep
         processSleep();
         
    }
	
	return 0;
}





