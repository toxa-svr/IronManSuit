// ********************************************************************
//  Debug output and Asserts
// ********************************************************************
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_trace.h"

// ****************************************************************************
// TRACE 
// ****************************************************************************
#ifdef USE_TRACE
 
    #define TMP_LOG_BUFF_SIZE    128 
    // Buffer to create formatted string before sending to uart
    uint8_t tmpLogBuffer[TMP_LOG_BUFF_SIZE];
   

    void __trace_handler(uint8_t * str_p, ...)
    {
        uint8_t size;
        
        va_list argptr;
        va_start(argptr, str_p);
        size = vsprintf(tmpLogBuffer, str_p, argptr); 
        va_end(argptr);
        
        // Send buffer to uart
        BSP_TO_UART(tmpLogBuffer, size); 
        BSP_TO_UART("\n\r", 2); 
    }
         
    
    void __dump_handler(uint8_t * data_p, uint8_t size) 
    {
        uint8_t * str_tmp_p = tmpLogBuffer;
        uint8_t   i;
        
        // print data
        if (data_p && size) {
            if (size * 3 > (TMP_LOG_BUFF_SIZE - 2)) {
                size = (TMP_LOG_BUFF_SIZE - 2) / 3; 
            }
        
            for (i = 0; i < size; i++) {
                sprintf(str_tmp_p, "%02X ", data_p[i]); // put 3 characters + nul-terminator
                str_tmp_p += 3;
            }
        }
        
        *(str_tmp_p++) = 0x0A; // \n
        *(str_tmp_p++) = 0x0D; // \r
        
        // Send buffer to uart
        BSP_TO_UART(tmpLogBuffer, (uint8_t)(size * 3 + 2));  
    }
     
#endif


// ****************************************************************************
// ASSERT 
// If any LEDs enabled, assert blinks all LEDs with given period in milliseconds.
// If UART enabled, assert puts given text into UART.
// ****************************************************************************
#ifdef USE_ASSERT

    inline void __assert_handler(uint16_t ms, uint8_t * data_p, uint8_t len, const uint8_t * file_str_p, const uint8_t * line_str_p) 
    {
        __disable_interrupt();
        
        #ifdef UART_TX_ENABLED
            BSP_uart_send_no_isr("\n\r\n\rASSERT: ", 12);  
            BSP_uart_send_no_isr(file_str_p, (uint8_t)strlen(file_str_p));
            BSP_uart_send_no_isr(" ", 1);
            BSP_uart_send_no_isr(line_str_p, (uint8_t)strlen(line_str_p));
            if (data_p && len) {
                BSP_uart_send_no_isr("\n\r", 2);
                BSP_uart_send_no_isr(data_p, len);
            }
            BSP_uart_send_no_isr("\n\r\n\r", 4);
        #endif
                 
                
        #if (LEDS_NUM > 0)
            BSP_LEDS_OFF(); 
            while(1){ BSP_LEDS_TOGGLE(); _delay_ms(ms); }
        #else          
            while(1){};
        #endif    
           
    }
    
#endif
