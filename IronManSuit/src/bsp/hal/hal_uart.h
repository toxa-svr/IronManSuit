// ****************************************************************************
// Hardware access layer for ATmega328p
// ****************************************************************************
// USART as UART
//
// To enable UART, in external file must be defined:
//    UART_TX_ENABLED
//    UART_RX_ENABLED
//    UART_BAUD_BPS
//    BSP_SYS_CLK_HZ
//
// Hardware UART pins for current MCU:  
//    PD1 - tx
//    PD0 - rx
// ****************************************************************************

#ifndef HAL_UART
#define HAL_UART

#include <avr/io.h>
#include "bsp.h"


// ****************************************************************************
// Pin assignment
// Hardware UART RX and RX can't be relocated to another pins:
// PD1 - tx
// PD0 - rx
// **************************************************************************** 
// UART TX - PD1 (hardware uart on ATmega328p)
#define UART_TX_PORT            PORTD
#define UART_TX_DDR             DDRD
#define UART_TX_PIN             PIND
#define UART_TX_BIT             1
       
// UART RX - PD0 (hardware uart on ATmega328p) 
#define UART_RX_PORT            PORTD
#define UART_RX_DDR             DDRD
#define UART_RX_PIN             PIND
#define UART_RX_BIT             0
    

// ****************************************************************************
// These macros check all needed UART definitions (pin assignment, baud rate, buffers)
// ****************************************************************************  

// Check TX pin assignment 
#ifdef UART_TX_ENABLED
    #if (((defined UART_TX_PORT) + (defined UART_TX_DDR) + (defined UART_TX_PIN) + (defined UART_TX_BIT)) != 4)
        #error "ERROR: For UART TX should be defined 4 params: UART_TX_PORT, UART_TX_DDR, UART_TX_PIN, UART_TX_BIT"   
    #endif  
    
    #ifndef UART_RX_ENABLED
        #warning "WARNING: UART TX enabled without UART RX"
    #endif    
#endif 

// Check RX pin assignment
#ifdef UART_RX_ENABLED
    #if (((defined UART_RX_PORT) + (defined UART_RX_DDR) + (defined UART_RX_PIN) + (defined UART_RX_BIT)) != 4)
        #error "ERROR: For UART RX should be defined 4 params: UART_RX_PORT, UART_RX_DDR, UART_RX_PIN, UART_RX_BIT"   
    #endif 
    
    #ifndef UART_TX_ENABLED
        #warning "WARNING: UART RX enabled without UART TX"
    #endif 
#endif  

// Check clock freq
#ifndef BSP_SYS_CLK_HZ    
    #error "ERROR: No BSP_SYS_CLK_HZ definiton" 
#endif    

// Neither TX nor RX enabled
#if (!defined(UART_TX_ENABLED) && !defined(UART_RX_ENABLED))
    #warning "WARNING: UART is disabled"
#endif



// ****************************************************************************
// Special macros for UART control
// **************************************************************************** 

//-------------------------------------------------------------------------------
// Pins
#define UART_TX_PIN_INIT()          HAL_GPIO_INIT_OUT_UP  (UART_TX_PORT, UART_TX_DDR, UART_TX_BIT); 
#define UART_RX_PIN_INIT()          HAL_GPIO_INIT_INP_ZZ  (UART_RX_PORT, UART_RX_DDR, UART_RX_BIT); 

#define UART_TX_PIN_DEINIT()        HAL_GPIO_INIT_INP_ZZ  (UART_TX_PORT, UART_TX_DDR, UART_TX_BIT); 
#define UART_RX_PIN_DEINIT()        HAL_GPIO_INIT_INP_ZZ  (UART_RX_PORT, UART_RX_DDR, UART_RX_BIT);

//-------------------------------------------------------------------------------
// Baud Rate Register - UBRR0
// UBRR0[11..0] = (fclk / (16 * BAUD_BPS)) - 1
#define UBRR_VAL(bps)      ((BSP_SYS_CLK_HZ / (16 * bps)) - 1)
#define UART_SET_BAUD(bps)   {  UBRR0L = (UBRR_VAL(bps)     ) & 0xFF;     \
                                UBRR0H = (UBRR_VAL(bps) >> 8) & 0x0F;  }   
                                           
//-------------------------------------------------------------------------------
// Status register UCSR0A:  
#define UART_IRQ_RX_FLAG_IS_UP()    (UCSR0A & (1<<RXC0))
#define UART_IRQ_TX_FLAG_IS_UP()    (UCSR0A & (1<<TXC0))
#define UART_IRQ_RX_FLAG_CLR()      {  /* automatically*/   }  // This flag cleared when the receive buffer is empty
                                                               //   (i.e., does not contain any unread data).
#define UART_IRQ_TX_FLAG_CLR()      { UCSR0A |= (1<<TXC0);  }  // The TXCn Flag bit is automatically cleared when a
                                                               //   transmit complete interrupt is executed, 
                                                               //   or it can be cleared by writing a one to its bit location.

//-------------------------------------------------------------------------------
// Control register UCSR0C:  UMSEL1 UMSEL0 00 - async, UPM0 USBS0 UCSZ02 UCSZ01 UCSZ00 00011 - N1
// Control register UCSR0B:  UCSZ02 0 - 8N1
#define UART_ENABLE()               {  /* all default velues  */ } 
#define UART_DISABLE()              {  /* no disable for uart */ }  
	
//-------------------------------------------------------------------------------
// Control register UCSR0B:  RXEN0 1 - rx enable, TXEN0 1 - tx enable
#define UART_RX_ENABLE()            { UCSR0B |=  (1<<RXEN0); }
#define UART_TX_ENABLE()            { UCSR0B |=  (1<<TXEN0); }
#define UART_RX_DISABLE()           { UCSR0B &= ~(1<<RXEN0); }
#define UART_TX_DISABLE()           { UCSR0B &= ~(1<<TXEN0); }

//-------------------------------------------------------------------------------
// Control register UCSR0B: RXCIE0 1 - rx interrupt enable, TXCIE0 1 - tx interrupt enable  
#define UART_IRQ_RX_ENABLE()        { UCSR0B |=  (1 << RXCIE0); }
#define UART_IRQ_TX_ENABLE()        { UCSR0B |=  (1 << TXCIE0); }
#define UART_IRQ_RX_DISABLE()       { UCSR0B &= ~(1 << RXCIE0); }
#define UART_IRQ_TX_DISABLE()       { UCSR0B &= ~(1 << TXCIE0); }
 
//-------------------------------------------------------------------------------
// Transmit - Receive
#define UART_TX(byte)             { UDR0 = byte; } 
#define UART_RX(byte)             { byte = UDR0; } 


//-------------------------------------------------------------------------------
// If mcu has only one interrupt for both TX and RX
//#define UART_ISR_SINGLE  
//#define UART_ISR_VECTOR  LIN_TC // Vector name for transfer complete interrupt 
  
// If mcu has separated interrupts for TX and RX
#define UART_ISR_TXRX
#define UART_ISR_VECTOR_TX   USART_TX_vect  // Vector name for TX complete interrupt 
#define UART_ISR_VECTOR_RX   USART_RX_vect  // Vector name for RX complete interrupt 


#if ((defined UART_ISR_SINGLE) && (defined UART_ISR_TXRX)) 
  #error "ERROR: Select UART_ISR_SINGLE or UART_ISR_TXRX"
#elif ((defined UART_ISR_SINGLE) && !(defined UART_ISR_VECTOR))
  #error "ERROR: No definition for UART interrupt vector"
#elif ((defined UART_ISR_TXRX) && !((defined UART_ISR_VECTOR_TX) && (defined UART_ISR_VECTOR_RX)))
  #error "ERROR: No definition for UART TX and RX interrupt vectors"
#endif


#endif // HAL_UART


   