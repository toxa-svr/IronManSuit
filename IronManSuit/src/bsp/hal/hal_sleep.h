// ****************************************************************************
// Hardware access layer for ATmega328p
// ****************************************************************************
//  Sleep modes and power reduction
//
// ****************************************************************************

#ifndef HAL_SLEEP
#define HAL_SLEEP

#include <avr/io.h>
//#include "bsp.h"




// **************************************************************************
// SLEEP
// **************************************************************************
#define BSP_SLEEP()  do { __asm__ __volatile__ ("sleep"); } while (0)


#define SLEEP_IDLE_MODE()                { SMCR = (1<<SE);                       BSP_SLEEP(); SMCR = 0; }
#define SLEEP_POWER_DOWN_MODE()          { SMCR = (1<<SE) | (1<<SM1);            BSP_SLEEP(); SMCR = 0; }
#define SLEEP_POWER_SAVE_MODE()          { SMCR = (1<<SE) | (1<<SM1) | (1<<SM0); BSP_SLEEP(); SMCR = 0; }
#define SLEEP_STANDBY_MODE()             { SMCR = (1<<SE) | (1<<SM2) | (1<<SM1); BSP_SLEEP(); SMCR = 0; }
#define SLEEP_ADC_NOISE_REDUCTION_MODE() { SMCR = (1<<SE) | (1<<SM0);            BSP_SLEEP(); SMCR = 0; }



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

#define POWER_ALL_ENABLE()       {PRR = 0;}
#define POWER_ALL_DISABLE()      {PRR = 0xFF;} // all power reduction bits to 1

#define POWER_UART_ENABLE()      {PRR &= ~(1<<PRUSART0);}
#define POWER_UART_DISABLE()     {PRR |= (1<<PRUSART0);}

#define POWER_ADC_ENABLE()       {PRR &= ~(1<<PRADC);}
#define POWER_ADC_DISABLE()      {PRR |= (1<<PRADC);}

#define POWER_TWI_ENABLE()       {PRR &= ~(1<<PRTWI);}
#define POWER_TWI_DISABLE()      {PRR |= (1<<PRTWI);}

#define POWER_TIMER0_ENABLE()    {PRR &= ~(1<<PRTIM0);}
#define POWER_TIMER0_DISABLE()   {PRR |= (1<<PRTIM0);}

#define POWER_TIMER1_ENABLE()    {PRR &= ~(1<<PRTIM1);}
#define POWER_TIMER1_DISABLE()   {PRR |= (1<<PRTIM1);}

#define POWER_TIMER2_ENABLE()    {PRR &= ~(1<<PRTIM2);}
#define POWER_TIMER2_DISABLE()   {PRR |= (1<<PRTIM2);}

#define POWER_PSC_ENABLE()       {PRR &= ~(1<<PRPSC);}
#define POWER_PSC_DISABLE()      {PRR |= (1<<PRPSC);}

#define POWER_SPI_ENABLE()       {PRR &= ~(1<<PRSPI);}
#define POWER_SPI_DISABLE()      {PRR |= (1<<PRSPI);}


// **************************************************************************
// ASYNCHRONOUS TIMER
// Hardware timer with external clock oscillator  
// **************************************************************************
#define SLEEP_TIMER_ENABLED


// Частота                       32768 Гц
// Период одного тика            30,517578125 мкс

// Предделитель                  1024
// Период одного тика            31250 мкс
// Значение компаратора 8сек     256    

// Предделитель                  256           
// Период одного тика            7812,5 мкс
// Значение компаратора 2сек     256           

// Предделитель                  128           
// Период одного тика            3906,25 мкс
// Значение компаратора 1сек     256           

// Предделитель                  64            
// Период одного тика            1952,125 мкс
// Значение компаратора 500мс    256           

// Предделитель                  32        
// Период одного тика            976,5625 мкс
// Значение компаратора 250мс    256        



// ----------------------------------------------------------------------------
// Settings for prescaller
#define SLEEP_TIMER_PRESCALLER_1024  ((1<<CS22) | (1<<CS21) | (1<<CS20))   // 1024

#define SLEEP_TIMER_TICK_US      31250UL  // single tick on prescaller 1024
#define SLEEP_TIMER_LONG_TICK_MS 7500UL   // 1000ms or 4000ms or 7500ms

#if (SLEEP_TIMER_LONG_TICK_MS == 1000) 
    #define SLEEP_TIMER_LONG_COMPAR     32
#elif (SLEEP_TIMER_LONG_TICK_MS == 4000)
    #define SLEEP_TIMER_LONG_COMPAR     128
#elif (SLEEP_TIMER_LONG_TICK_MS == 7500)
    #define SLEEP_TIMER_LONG_COMPAR     240
#endif

//----------------------------------------------------------------------------
// To start timer in asynchronous mode:
//	Disable the Timer/Counter2 interrupts by clearing OCIE2x and TOIE2.
//	Select clock source by setting AS2 as appropriate.
//	Write new values to TCNT2, OCR2x, and TCCR2x.
//	To switch to asynchronous operation: Wait for TCN2xUB, OCR2xUB, and TCR2xUB.
//	Clear the Timer/Counter2 Interrupt Flags.
//	Enable interrupts, if needed.
#define SLEEP_TIMER_START(compar) {	                                                    \
										TIMSK2 = 0;                   	        		\
										ASSR = (1<<AS2);          						\
										TCNT2 = 0;                						\
										while(ASSR & TCN2UB) {}                 		\
										TCCR2A = (1<<WGM21);     						\
										while(ASSR & TCR2AUB) {}                		\
										OCR2A = compar;                         		\
										while(ASSR & OCR2AUB) {}                		\
										TCCR2B = SLEEP_TIMER_PRESCALLER_1024;       	\
										while(ASSR & TCR2BUB) {}                		\
                                        TIFR2 = ((1<<OCF2A) | (1<<OCF2B) | (1<<TOV2)); 	\
									}                    
                                                                                       	
//----------------------------------------------------------------------------
// Stop timer
#define SLEEP_TIMER_STOP()          {	                                                \
										TIMSK2 = 0;                   	        		\
										ASSR = (1<<AS2);          						\
										TCCR2B = 0;                         	     	\
										while(ASSR & TCR2BUB) {}                		\
                                        TIFR2 = ((1<<OCF2A) | (1<<OCF2B) | (1<<TOV2)); 	\
									} 
  
//----------------------------------------------------------------------------
// Enable/disable interrupt                               
#define SLEEP_TIMER_ISR_ENABLE()    {TIMSK2 = (1<<OCIE2A);}
#define SLEEP_TIMER_ISR_DISABLE()   {TIMSK2 = 0;}


//----------------------------------------------------------------------------
// Vector name for comparator interrupt
#define SLEEP_TIMER_ISR_VECTOR   TIM2_COMPA  



#endif // HAL_SLEEP


   