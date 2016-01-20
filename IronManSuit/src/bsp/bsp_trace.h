// ****************************************************************************
// Debug output and Asserts
// ****************************************************************************
//
// To enable TRACE output and ASSERTs, in external file must be defined:
//    USE_ASSERT
//    USE_TRACE
//
// ****************************************************************************
#ifndef BSP_TRACE_H
#define BSP_TRACE_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "bsp.h"
#include "bsp_gpio.h"

#ifdef UART_TX_ENABLED
    #include "bsp_uart.h"
#endif


// ****************************************************************************
// TRACE
//
// BSP_TO_UART(data_p, size)      -  send raw data to uart, no LRLF
// BSP_STR_TO_UART(str_p)         -  send null-treminated <str_p> to UART, no LRLF 
// BSP_TRACE(...)                 -  create formatted string and call BSP_TO_UART, with LRLF
// BSP_DUMP(data_p, size)         -  create hex-view of data <data_p> and call BSP_TO_UART, with LRLF
// ****************************************************************************
#ifdef USE_TRACE 
    #ifndef UART_TX_ENABLED  // undef USE_TRACE if there is no uart for output
        #warning "WARNING: No UART TX for TRACE output. Trace will be disabled"
        #undef USE_TRACE
    #endif
#endif
    
#ifdef USE_TRACE

    void __trace_handler(uint8_t * str_p, ...);
    void __dump_handler(uint8_t * data_p, uint8_t size);
           
    #define BSP_TO_UART(data_p, size)     { BSP_uart_send((char*)data_p, size); }
    #define BSP_STR_TO_UART(str_p)        { BSP_uart_send((char*)str_p, strlen(str_p)); }
    #define BSP_TRACE(str_p, ...)         { __trace_handler((char*)str_p, ##__VA_ARGS__); }
    #define BSP_DUMP(data_p, size)        { __dump_handler((char*)data_p, size); }          
                       
#else
    #define BSP_TO_UART(data_p, size)     {}  // empty 
    #define BSP_STR_TO_UART(str_p)        {}  // empty 
    #define BSP_TRACE(str_p, ...)         {}  // empty
    #define BSP_DUMP(data_p, size)        {}  // empty 
#endif


// ****************************************************************************
// ASSERT 
//
// If any LEDs enabled, assert blinks all LEDs with given period in milliseconds.
// If UART enabled, assert puts file name, string number and given data <data_p> into UART.
// ****************************************************************************
#define LINE_TO_STR(line)    #line
#define LINE_NUMBER          LINE_TO_STR(__LINE__)


#ifdef USE_ASSERT
    void __assert_handler(uint16_t ms, uint8_t * data_p, uint8_t len, const uint8_t * file_str_p, const uint8_t * line_str_p);
                   
    #ifdef UART_TX_ENABLED
        // If UART Tx can be used - ASSERT with text and blinking
        #define BSP_ASSERT(expr)                   { if (!(expr)) {__assert_handler(50, NULL,     0, __FILE__, LINE_NUMBER); } }
        #define BSP_ASSERT_LED(expr,ms)            { if (!(expr)) {__assert_handler(ms, NULL,     0, __FILE__, LINE_NUMBER); } }
        #define BSP_ASSERT_DATA(expr,data_p,len)   { if (!(expr)) {__assert_handler(50, (uint8_t*)data_p, len, __FILE__, LINE_NUMBER); } }

    #elif (LEDS_NUM > 0)
        #warning "WARNING: No UART TX to indicate ASSERT. Just LEDs blinking." 
        // If LEDs can be used - ASSERT with blinking
        #define BSP_ASSERT(expr)                   { if (!(expr)) {__assert_handler(50, NULL, 0, NULL, NULL); } }
        #define BSP_ASSERT_LED(expr,ms)            { if (!(expr)) {__assert_handler(ms, NULL, 0, NULL, NULL); } }
        #define BSP_ASSERT_DATA(expr,data_p,len)   { if (!(expr)) {__assert_handler(50, NULL, 0, NULL, NULL); } }
         
    #else 
        #warning "No LEDs or UART TX to indicate ASSERT. Just stop programm."
        #define BSP_ASSERT(expr)                   {}  // empty assert 
        #define BSP_ASSERT_LED(expr,ms)            {}  // empty assert
        #define BSP_ASSERT_DATA(expr,data_p,len)   {}  // empty assert
    #endif
         
#else 
    #define BSP_ASSERT(expr)                   {}  // empty assert 
    #define BSP_ASSERT_LED(expr,ms)            {}  // empty assert
    #define BSP_ASSERT_DATA(expr,data_p,len)   {}  // empty assert 
#endif
   

#endif  // BSP_TRACE_H
