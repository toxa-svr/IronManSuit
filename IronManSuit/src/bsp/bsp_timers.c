// ****************************************************************************
// Software timers
// ****************************************************************************
//
// In external file must be defined:
//   SWTIMERS_MAX
//   TIMER_ISR_PERIOD_MSEC
//
// ****************************************************************************
#include <stdint.h>
#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_trace.h"
#include "bsp_timers.h"


// ----------------------------------------------------------------------------
// Структура программного таймера
typedef struct {
   uint8_t          is_stopped; // 0 - таймер запущен; 1 - таймер остановлен 
   uint8_t          is_fired;   // 0 - таймер не срабатывал; 1 - таймер сработал        
   swTimerMode_t	mode;       // режим (однократный или периодический)   
   uint32_t     	counter;    // счетчик прерываний аппаратного таймера
   uint32_t     	threshold;  // значение порога счетчика для срабатывания
   swTimerHandler   handler;    // указатель на функцию обработчик
} swTimer_t;

// ----------------------------------------------------------------------------
// Масссив программных таймеров
volatile swTimer_t swTimers[SWTIMERS_MAX];


// ----------------------------------------------------------------------------
// Инициализация - обнуление состояний, запуск аппаратного таймера
void BSP_timer_init(void) 
{
    uint8_t i;
    for (i = 0; i < SWTIMERS_MAX; i++) {
        swTimers[i].is_stopped = 1; 
    }
    
    // Запуск аппаратного таймера
    TIMER_INIT();
}


// ----------------------------------------------------------------------------
// Запуск таймера на миллисекундный интервал от SWTIMERS_MIN_TIME до SWTIMERS_MAX_TIME [ms]
void BSP_timer_start_ms(uint8_t id, uint32_t timeout_ms, 
                            swTimerMode_t mode, swTimerHandler handler) 
{
    BSP_USE_CRITICAL();

    BSP_ASSERT(id < SWTIMERS_MAX);                 // wrong timer id
    BSP_ASSERT(timeout_ms >= SWTIMERS_MIN_TIME);   // wrong timeout    
    BSP_ASSERT(timeout_ms <= SWTIMERS_MAX_TIME);   // wrong timeout    
        
    // Установка состояния
    BSP_CRITICAL(
        swTimers[id].is_stopped = 0;
        swTimers[id].is_fired = 0;  
        swTimers[id].counter = 0;  
        swTimers[id].threshold = timeout_ms / TIMER_ISR_PERIOD_MSEC;
        swTimers[id].handler = handler;          
        swTimers[id].mode = mode; 
    );
}


// ----------------------------------------------------------------------------
// Проверка состояния таймера
// Возвращает: 0 - остановлен, 1 - запущен, 0xFF - не был создан
uint8_t BSP_timer_is_run(uint8_t id)
{
    uint8_t is_stopped;
    BSP_USE_CRITICAL();
    
    BSP_ASSERT(id < SWTIMERS_MAX); // wrong timer id
    
    BSP_CRITICAL(
        is_stopped = swTimers[id].is_stopped;
    );
    
    return !(is_stopped);
}


// ----------------------------------------------------------------------------
// Остановка таймера
// Возвращает: время [ms], прошедшее от запуска таймера (или 0, если таймер не был запущен)
uint32_t BSP_timer_stop(uint8_t id) 
{
    uint32_t time;
    
    BSP_USE_CRITICAL();
    BSP_ASSERT(id < SWTIMERS_MAX); // wrong timer id
            
    // Установка состояния
    BSP_CRITICAL( 
        time = swTimers[id].counter;
        swTimers[id].is_stopped = 1;
        swTimers[id].is_fired = 0;  
        swTimers[id].counter = 0;  
        swTimers[id].threshold = 0;
    );
    return (time * TIMER_ISR_PERIOD_MSEC);
}


// ----------------------------------------------------------------------------
// Проверка флага срабатывания одного таймера и вызов обработчика
// После проверки флаг срабатывания сбрасывается 
void BSP_timer_process(uint8_t id) 
{
    uint8_t is_fired = 0;
    BSP_USE_CRITICAL();

    BSP_ASSERT(id < SWTIMERS_MAX); // wrong timer id
    
    BSP_CRITICAL(
        if (swTimers[id].is_fired) {
            swTimers[id].is_fired = 0;
            is_fired = 1;
        }
    );
    
    if (is_fired) {
        // Обработка срабатывания 
        if (swTimers[id].handler) {
            (swTimers[id].handler)();
        }
    }
}


// ----------------------------------------------------------------------------
// Последовательная проверка флага срабатывания для всех таймеров и вызов обработчиков
// После проверки флаг срабатывания сбрасывается
void BSP_timer_process_all(void) 
{
    uint8_t i;
    for (i = 0; i < SWTIMERS_MAX; i++) {
        BSP_timer_process(i);
    } 
}




// ****************************************************************************
// Прерывание
// ****************************************************************************

ISR (TIMER_ISR_VECTOR)
//interrupt [TIMER_ISR_VECTOR] void BSP_timer_compA_isr(void)
{
    uint8_t i;

    for(i = 0; i< SWTIMERS_MAX; i++){
        if (!swTimers[i].is_stopped){
            swTimers[i].counter++;
            if (swTimers[i].counter >= swTimers[i].threshold) {
                // Таймер сработал
                swTimers[i].counter = 0;
                swTimers[i].is_fired = 1;
                
                // Если таймер одноразовый - остановить
                if (swTimers[i].mode == SWTIMER_SINGLE) {
                    swTimers[i].is_stopped = 1;
                    swTimers[i].threshold = 0;
                } 
            }
        }
    }
	
    return;
}