// ****************************************************************************
//  Sleep timer for power save modes
// ****************************************************************************
//
// ****************************************************************************

#include <stdint.h> 
#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_trace.h"
#include "bsp_sleep.h"


#ifdef SLEEP_TIMER_ENABLED // only if sleep timer available in HAL

// ----------------------------------------------------------------------------
// Struct for sleep timer
typedef struct {
   volatile uint8_t  is_started;            
   volatile uint8_t  is_fired;
   volatile uint8_t  is_started_short;
   volatile uint8_t  is_fired_temp;
   volatile uint32_t long_intervals_cnt;     // counter for long timer's intervals
   volatile uint32_t long_intervals_thr;     // threshold to start timer with long interval
   volatile sleepTimerHandler handler;       // указатель на функцию обработчик
} sleepTimer_t;
   
volatile sleepTimer_t sleepTimer; 



// ----------------------------------------------------------------------------
// Sleep timer initialization - first synchronization between system 
//   clock and asynchronous clock domain 
// ----------------------------------------------------------------------------
void BSP_sleep_timer_init (void) 
{                   
    SLEEP_TIMER_START(1); 
    // sleep timer's interrupts are still disabled here
    SLEEP_TIMER_STOP();
}


// ----------------------------------------------------------------------------
// Start sleep timer with period in milliseconds
// Timeout = comp_short_local  +  SLEEP_TIMER_LONG_TICK_MS * thr_long_local
// ----------------------------------------------------------------------------
void BSP_sleep_timer_start_ms(uint32_t timeout_ms, sleepTimerHandler handler)
{
    BSP_USE_CRITICAL(); 

    // Calculate thresholds  
    uint32_t thr_long_local  = timeout_ms / SLEEP_TIMER_LONG_TICK_MS;  // how many long intervals in timeout_ms  
    uint8_t short_compar  = (uint8_t)((timeout_ms - thr_long_local * SLEEP_TIMER_LONG_TICK_MS) * 1000UL / SLEEP_TIMER_TICK_US);
        
    BSP_TRACE("sleep timer %dms: %d x %dms + %dms", timeout_ms, thr_long_local, SLEEP_TIMER_LONG_TICK_MS, 
                                                        (timeout_ms - thr_long_local * SLEEP_TIMER_LONG_TICK_MS));
                                                                            
    BSP_CRITICAL(  
        sleepTimer.is_started = 1;
        sleepTimer.is_fired = 0;
        sleepTimer.is_started_short = (short_compar) ? 1 : 0; 
        sleepTimer.is_fired_temp = 0;                                               
        sleepTimer.long_intervals_cnt = 0;
        sleepTimer.long_intervals_thr = thr_long_local;
        sleepTimer.handler = handler;          
    );
    
    
    // Start timer with short interval and enable interrupts        
    // If short interval is zero - start timer with long interval       
    if (short_compar == 0) { 
        short_compar = SLEEP_TIMER_LONG_COMPAR;     
    }

    SLEEP_TIMER_START(short_compar);   
    SLEEP_TIMER_ISR_ENABLE(); 
}
    
// ----------------------------------------------------------------------------
// Check sleep timer and call event handler
// ----------------------------------------------------------------------------
void BSP_sleep_timer_process() 
{
    BSP_USE_CRITICAL();
        
    
//    BSP_CRITICAL_BEGIN();
//    if (sleepTimer.is_started && sleepTimer.is_fired_temp) {
//        sleepTimer.is_fired_temp = 0; 
//        BSP_CRITICAL_END(); 
//        BSP_TRACE("sleep timer temp fired", 0);
//    }
//    else {
//        BSP_CRITICAL_END();
//    }      
//    

    BSP_CRITICAL_BEGIN();
    if (sleepTimer.is_started && sleepTimer.is_fired) {
        sleepTimer.is_fired = 0;
        sleepTimer.is_started = 0;
        BSP_CRITICAL_END();
        
        BSP_TRACE("sleep timer fired", 0);
                          
        // Process event 
        if (sleepTimer.handler) {
            (sleepTimer.handler)();
        }
    } 
    else {
        BSP_CRITICAL_END();
    }
}  


// ----------------------------------------------------------------------------
// Sleep timer interrupt - first interrupt after wakeup
// ----------------------------------------------------------------------------
interrupt [SLEEP_TIMER_ISR_VECTOR] void BSP_sleep_timer_compA_isr(void)
{    
    if (sleepTimer.is_started && (!sleepTimer.is_fired)) {
        // If it was short interval
        if (sleepTimer.is_started_short) {
            sleepTimer.is_started_short = 0;
            sleepTimer.is_fired_temp = 1;  
            // Stop hw timer
            SLEEP_TIMER_ISR_DISABLE(); 
            SLEEP_TIMER_STOP();
         
            // If long intervals are expected
            if (sleepTimer.long_intervals_thr) {
                // Restart hw timer with long interval 
                SLEEP_TIMER_START(SLEEP_TIMER_LONG_COMPAR);
                SLEEP_TIMER_ISR_ENABLE();
            }
            else {
                sleepTimer.is_fired = 1;
            }
        }
         
        // If it was long interval
        else {
            sleepTimer.long_intervals_cnt++; 
            sleepTimer.is_fired_temp = 1;
            
            // All long intervals are finished
            if (sleepTimer.long_intervals_cnt >= sleepTimer.long_intervals_thr) {
                sleepTimer.is_fired = 1; 
                sleepTimer.is_fired_temp = 0;
                // Stop hw timer
                SLEEP_TIMER_ISR_DISABLE();  
                SLEEP_TIMER_STOP();
            }
        }
    }
        
    return;
}




#endif  // SLEEP_TIMER_ENABLED



