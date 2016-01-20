// ****************************************************************************
// Hardware access layer for ATmega328p
// ****************************************************************************
// Hardware external interrupts
//
// To enable External interrupt, in external file must be defined:
//    EXTINT0_ENABLED
//    EXTINT1_ENABLED
//
//
// Hardware EXTINT pins for current MCU:  
//    PD2 - int0
//    PD3 - int1
// ****************************************************************************

#ifndef HAL_EXTINT
#define HAL_EXTINT


#include <avr/io.h>
#include "bsp.h"


// ****************************************************************************
// Pin assignment
// External interrupts are hardware and can't be relocated to another pins:
//    PD2 - int0
//    PD3 - int1
// **************************************************************************** 
#define EXTI0_PORT    PORTD
#define EXTI0_DDR     DDRD
#define EXTI0_BIT     2

#define EXTI1_PORT    PORTD
#define EXTI1_DDR     DDRD
#define EXTI1_BIT     3
    
	
// ****************************************************************************
// These macros check all needed EXTINT definitions (pin assignment)
// and add EXTINT control macros
// **************************************************************************** 
#ifdef EXTINT0_ENABLED
    #if (((defined EXTI0_PORT) + (defined EXTI0_DDR) + (defined EXTI0_BIT)) != 3)                
        #error "ERROR: For INT0 must be defined 3 params: EXTIn_PORT, EXTIn_DDR, EXTIn_BIT"
    #endif
	// Init extint
	#define EXTINT0_INIT(rising_edge)   { HAL_GPIO_INIT_INP_ZZ(EXTI0_PORT, EXTI0_DDR, EXTI0_BIT);  \
                                          if (rising_edge) { EICRA |= (1 << ISC01)|(1 << ISC00); } \
                                          else             { EICRA |= (1 << ISC01); }                 }
	// Enable/disable
	#define EXTINT0_ON()    { EIMSK |= (1 << INT0);  } 
	#define EXTINT0_OFF()   { EIMSK &= ~(1 << INT0); } 
#endif

//-------------------------------------------------------------------------------

#ifdef EXTINT1_ENABLED
    #if (((defined EXTI1_PORT) + (defined EXTI1_DDR) + (defined EXTI1_BIT)) != 3)                
        #error "ERROR: For INT1 must be defined 3 params: EXTIn_PORT, EXTIn_DDR, EXTIn_BIT"
    #endif
	// Init extint
	#define EXTINT1_INIT(rising_edge)   { HAL_GPIO_INIT_INP_ZZ(EXTI1_PORT, EXTI1_DDR, EXTI1_BIT);  \
                                          if (rising_edge) { EICRA |= (1 << ISC11)|(1 << ISC10); } \
                                          else             { EICRA |= (1 << ISC11); }                 }
	// Enable/disable
	#define EXTINT1_ON()    { EIMSK |= (1 << INT1);  } 
	#define EXTINT1_OFF()   { EIMSK &= ~(1 << INT1); } 
#endif



//-------------------------------------------------------------------------------
#if (!defined(EXTINT0_ENABLED) && !defined(EXTINT1_ENABLED))
    #warning "WARNING: All EXT INT are disabled "
#endif 




	
#endif // HAL_EXTINT


   