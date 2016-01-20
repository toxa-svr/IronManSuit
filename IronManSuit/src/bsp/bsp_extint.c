// ****************************************************************************
// Hardware EXTERNAL INTERRUPTS
// ****************************************************************************

#include <stdint.h>
#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_extint.h"


// ****************************************************************************
// External Interrupt controls
// ****************************************************************************
void BSP_extint_init(uint8_t chan, uint8_t rising_edge) 
{
#ifdef EXTINT0_ENABLED
    if (chan == 0) { EXTINT0_INIT (rising_edge); }
#endif

#ifdef EXTINT1_ENABLED
    if (chan == 1) { EXTINT1_INIT(rising_edge); }
#endif  

#ifdef EXTINT2_ENABLED
    if (chan == 2) { EXTINT2_INIT(rising_edge); } 
#endif   

#ifdef EXTINT3_ENABLED
    if (chan == 3) { EXTINT3_INIT(rising_edge); } 
#endif                           
}


//-------------------------------------------------------------------------------
void BSP_extint_enable(uint8_t chan) 
{
#ifdef EXTINT0_ENABLED
    if (chan == 0) { EXTINT0_ON(); } 
#endif  
#ifdef EXTINT1_ENABLED
    if (chan == 1) { EXTINT1_ON();  } 
#endif  
#ifdef EXTINT2_ENABLED
    if (chan == 2) { EXTINT2_ON();  } 
#endif  
#ifdef EXTINT3_ENABLED
    if (chan == 3) { EXTINT3_ON();  } 
#endif      
}


//-------------------------------------------------------------------------------
void BSP_extint_disable(uint8_t chan) 
{
#ifdef EXTINT0_ENABLED
    if (chan == 0) { EXTINT0_OFF(); } 
#endif  
#ifdef EXTINT1_ENABLED
    if (chan == 1) { EXTINT1_OFF(); } 
#endif  
#ifdef EXTINT2_ENABLED
    if (chan == 2) { EXTINT2_OFF(); } 
#endif  
#ifdef EXTINT3_ENABLED
    if (chan == 3) { EXTINT3_OFF(); } 
#endif    
}



// ****************************************************************************
// INTn interrupt
// ****************************************************************************
#ifdef EXTINT0_ENABLED
    //interrupt  [EXT_INT0] void BSP_extint0_isr(void) { EXT_INT0_isr_handler(); }   
    ISR (INT0_vect)   { EXT_INT0_isr_handler(); } 
#endif

#ifdef EXTINT1_ENABLED
    // interrupt  [EXT_INT1] void BSP_extint1_isr(void) { EXT_INT1_isr_handler(); } 
    ISR (INT1_vect)   { EXT_INT1_isr_handler(); }    
#endif

#ifdef EXTINT2_ENABLED
    interrupt  [EXT_INT2] void BSP_extint2_isr(void) { EXT_INT2_isr_handler(); }    
#endif

#ifdef EXTINT3_ENABLED
    interrupt  [EXT_INT3] void BSP_extint3_isr(void) { EXT_INT3_isr_handler(); }    
#endif
