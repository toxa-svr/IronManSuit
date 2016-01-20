// ****************************************************************************
// Hardware access layer for ATmega328p
// ****************************************************************************
// 8-bit Timer/Counter 0
//
// In external file must be defined:
//    BSP_SYS_CLK_HZ
//
// ****************************************************************************

#ifndef HAL_TIMER
#define HAL_TIMER

#include <avr/io.h>
#include "bsp/bsp.h"



//----------------------------------------------------------------------------
// Period between two hw-timer interrupts [ms] - base tick for software timers
//----------------------------------------------------------------------------
#define TIMER_ISR_PERIOD_MSEC  10





//----------------------------------------------------------------------------
// Аппаратный таймер для обслуживания программных таймеров
// Вырабатывает прерывания с периодом TIMER_ISR_PERIOD_MSEC
//----------------------------------------------------------------------------
// Используется 8-битный таймер 0 с прерыванием компаратора А

// Частота                       1000000 Гц     (TIMER_CLK_HZ)
// Предделитель                  64             (TIMER_PRESCALLER)
// Период одного тика            64 мкс
// Значение компаратора          156            (TIMER_COMPARATOR)


#define TIMER_CLK_HZ                    BSP_SYS_CLK_HZ

#if (TIMER_CLK_HZ == 1000000UL)
    #define TIMER_PRESCALLER 64
    #define TIMER_COMPARATOR 156 
#elif (TIMER_CLK_HZ == 8000000UL)
    #define TIMER_PRESCALLER 256
    #define TIMER_COMPARATOR 156
#else 
    #error "ERROR: Missing declaration for TIMER_CLK_HZ (HW timer clock)" 
#endif       


// ----------------------------------------------------------------------------
// Macro to start hw-timer with prescaller
#if (TIMER_PRESCALLER == 1024)   
    #define TIMER_START()  { TCCR0B = (1<<CS02) | (1<<CS00); } 
#elif (TIMER_PRESCALLER == 256) 
    #define TIMER_START()  { TCCR0B = (1<<CS02); }
#elif (TIMER_PRESCALLER == 64) 
    #define TIMER_START()  { TCCR0B = (1<<CS01) | (1<<CS00); }
#else
    #error "ERROR: Missing declaration for TIMER_PRESCALLER (HW timer prescaller value)"
#endif  


// ----------------------------------------------------------------------------
// Macro for 8-bit hw-timer full initialization and start
//	TCCR0B = 0;                                     // Остановка таймера
//  TIFR0 = 0;                                      // Очистка всех флагов прерываний таймера
//  TIMSK0 = 0;                                     // Зпрет всех прерываний
//  TCNT0 = 0;                                      // Сброс счетчика
//	TCCR0A = (1<<WGM01);                            // Режим CTC - обнуление счетчика при срабатывании компратора, без вывода ШИМ
//  OCR0A = TIMER_COMPARATOR;                       // Установка компаратора А
//  TIMSK0 = (1<<OCIE0A);                           // Разрешение прерываний компаратора А
	
#define TIMER_INIT() 	  { TCCR0B = 0;                                    \
							TIMSK0 = 0;                                    \
							TCNT0 = 0;                                     \
							TCCR0A = (1<<WGM01);                           \
							OCR0A = TIMER_COMPARATOR;                      \
							TIMSK0 = (1<<OCIE0A);                          \
							TIMER_START(); }
                               
                            
//----------------------------------------------------------------------------
// Vector name for comparator interrupt 
#define TIMER_ISR_VECTOR   TIMER0_COMPA_vect 


#endif // HAL_TIMER


   