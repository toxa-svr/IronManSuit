// ****************************************************************************
// EXTERNAL INTERRUPTS
// ****************************************************************************
//
// To enable External interrupt, in external file must be defined:
//    EXTINT0_ENABLED
//    EXTINT1_ENABLED
//    EXTINT2_ENABLED
//    EXTINT3_ENABLED
//
// External functions (n = 0, 1, 2, 3) are required for interrupt handling:
//   void EXT_INTn_isr_handler(void)
//
// ****************************************************************************
#ifndef BSP_EXTINT_H
#define BSP_EXTINT_H

#include <stdint.h>
#include "bsp.h"


// ****************************************************************************
// External Interrupt controls
// ****************************************************************************

// Init external interrrupt hardware 
//   rising_edge = 1 to generate interrupt on rising edge 
//   rising_edge = 0 to generate interrupt on falling edge
void BSP_extint_init(uint8_t chan, uint8_t rising_edge);

void BSP_extint_enable(uint8_t chan);
void BSP_extint_disable(uint8_t chan);


// ****************************************************************************
// External functions for INTn interrupt handling
// ****************************************************************************
#ifdef EXTINT0_ENABLED
    extern void EXT_INT0_isr_handler(void);
#endif

#ifdef EXTINT1_ENABLED
    extern void EXT_INT1_isr_handler(void);    
#endif

#ifdef EXTINT2_ENABLED
    extern void EXT_INT2_isr_handler(void);    
#endif

#ifdef EXTINT3_ENABLED
    extern void EXT_INT3_isr_handler(void);    
#endif


#endif  // BSP_EXTINT_H
