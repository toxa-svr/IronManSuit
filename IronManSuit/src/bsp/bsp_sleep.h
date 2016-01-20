// ****************************************************************************
//  Sleep modes and power reduction
// ****************************************************************************

#ifndef BSP_SLEEP_H
#define BSP_SLEEP_H

#include "bsp.h"


// ****************************************************************************
// SLEEP
// ****************************************************************************

// MCU will be stay in sleep mode untill wake-up
#define BSP_IDLE_MODE()                SLEEP_IDLE_MODE();
#define BSP_POWER_DOWN_MODE()          SLEEP_POWER_DOWN_MODE();
#define BSP_STANDBY_MODE()             SLEEP_STANDBY_MODE();
#define BSP_ADC_NOISE_REDUCTION_MODE() SLEEP_ADC_NOISE_REDUCTION_MODE();

#ifdef SLEEP_POWER_SAVE_MODE
    #define BSP_POWER_SAVE_MODE()      SLEEP_POWER_SAVE_MODE();
#endif

// ****************************************************************************
// POWER REDUCTION
// ****************************************************************************
// To Disable clocks for unused blocks set 1 to corresponding bit of Power Reduction Register
// PRR - Power Reduction Register
//    PRADC Power Reduction ADC
//    PRLIN Power Reduction LIN UART
//    PRSPI Power Reduction Serial Peripheral Interface
//    PRTIM0 Power Reduction Timer/Counter0
//    PRTIM1 Power Reduction Timer/Counter1
//    PRPSC Power Reduction PSC
//    PRCAN Power Reduction CAN
#define BSP_POWER_ALL_DISABLE()      POWER_ALL_DISABLE();

#define BSP_POWER_UART_ENABLE()      POWER_UART_ENABLE();
#define BSP_POWER_UART_DISABLE()     POWER_UART_DISABLE();

#define BSP_POWER_ADC_ENABLE()       POWER_ADC_ENABLE();
#define BSP_POWER_ADC_DISABLE()      POWER_ADC_DISABLE();

#define BSP_POWER_TIMER1_ENABLE()    POWER_TIMER1_ENABLE();
#define BSP_POWER_TIMER1_DISABLE()   POWER_TIMER1_ENABLE();

#define BSP_POWER_SPI_ENABLE()       POWER_SPI_ENABLE();
#define BSP_POWER_SPI_DISABLE()      POWER_SPI_DISABLE();

#ifdef POWER_TWI_ENABLE
    #define BSP_POWER_TWI_ENABLE()       POWER_TWI_ENABLE();
#endif

#ifdef POWER_TWI_DISABLE
    #define BSP_POWER_TWI_DISABLE()      POWER_TWI_DISABLE();
#endif

#ifdef POWER_CAN_ENABLE
    #define BSP_POWER_CAN_ENABLE()       POWER_CAN_ENABLE();
#endif

#ifdef POWER_CAN_DISABLE
    #define BSP_POWER_CAN_DISABLE()      POWER_CAN_DISABLE();
#endif

#ifdef POWER_PSC_ENABLE 
    #define BSP_POWER_PSC_ENABLE()       POWER_PSC_ENABLE();
#endif
    
#ifdef POWER_PSC_DISABLE    
    #define BSP_POWER_PSC_DISABLE()      POWER_PSC_DISABLE();
#endif

// **************************************************************************
// ASYNCHRONOUS TIMER 2
// Hardware timer with external clock oscillator  
// **************************************************************************
#ifdef SLEEP_TIMER_ENABLED

    // Handler for sleep timer event
    typedef void (*sleepTimerHandler)(void);
                                     
    // Sleep timer initialization
    void BSP_sleep_timer_init (void);

    // Start sleep timer with period in milliseconds
    void BSP_sleep_timer_start_ms(uint32_t timeout_ms, sleepTimerHandler handler);
    
    // Check sleep timer and call event handler
    void BSP_sleep_timer_process();

#endif


#endif  // BSP_SLEEP_H
