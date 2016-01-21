// ****************************************************************************
//  Board support package
// ****************************************************************************
//
//  Board-specified definitions for peripheral port assignment
//
// ****************************************************************************
#ifndef BSP_H
#define BSP_H


// ****************************************************************************
// Board defines
// ****************************************************************************
#define BOARD_IRONMAN_SUIT

//#define USE_ASSERT
#define USE_TRACE
//#define USE_CONSOLE


// ****************************************************************************
// Iron Man suit board description
// ATmegam1
// Clock 2 MHz
// 1 UART   (debug trace)
// 1 ADC    (battery voltage)
// 1 EXTINT () 
// 1 PWM    (helmet servos)
// 5 LED    (left hand, right hand, chest, eyes, servo enable out)  
// ****************************************************************************
#ifdef BOARD_IRONMAN_SUIT
      
    // System clock
    #define BSP_SYS_CLK_HZ    1000000UL

    #ifndef F_CPU
        #error "ERROR: F_CPU must be defined in project settings"
    #endif
    
    #if (F_CPU != BSP_SYS_CLK_HZ)
        #error "ERROR: BSP_SYS_CLK_HZ must be the same as F_CPU"
    #endif


    // UART
    #define UART_TX_ENABLED
    //#define UART_RX_ENABLED
    #define UART_BAUD_BPS         2400UL

    // LED 0 - PB2 - (pin D10) - Eyes 
    #define LED0_PORT             PORTB
    #define LED0_PIN              PINB
    #define LED0_DDR              DDRB
    #define LED0_BIT              2           
    #define LED0_IS_ACTIVE_LOW    0   
  
    // LED 1 - PB1 - (pin D9) - Chest
    #define LED1_PORT             PORTB
    #define LED1_PIN              PINB
    #define LED1_DDR              DDRB
    #define LED1_BIT              1
    #define LED1_IS_ACTIVE_LOW    0

    // LED 2 - PD6 - (pin D6) - Left Hand
    #define LED2_PORT             PORTD
    #define LED2_PIN              PIND
    #define LED2_DDR              DDRD
    #define LED2_BIT              6
    #define LED2_IS_ACTIVE_LOW    0
    
    // LED 3 - PD5 - (pin D5) - Right Hand 
    #define LED3_PORT             PORTD
    #define LED3_PIN              PIND
    #define LED3_DDR              DDRD
    #define LED3_BIT              5
    #define LED3_IS_ACTIVE_LOW    0
    
    // LED 4 - PD4 - (pin D4) - Servo Enable
    #define LED4_PORT             PORTD
    #define LED4_PIN              PIND
    #define LED4_DDR              DDRD
    #define LED4_BIT              4
    #define LED4_IS_ACTIVE_LOW    1
    
    // LED 5 - PD7 - (pin D7) - Battery voltage measurement Enable
    #define LED5_PORT             PORTD
    #define LED5_PIN              PIND
    #define LED5_DDR              DDRD
    #define LED5_BIT              7
    #define LED5_IS_ACTIVE_LOW    0
        
    // LED 6 - PD5 - (pin D5) - Servo 1 pwm output
    #define LED6_PORT             PORTD
    #define LED6_PIN              PIND
    #define LED6_DDR              DDRD
    #define LED6_BIT              5
    #define LED6_IS_ACTIVE_LOW    0

/*	
	// LED 7 - PD7 - (pin D7) - Servo 2 pwm output
	#define LED5_PORT             PORTD
	#define LED5_PIN              PIND
	#define LED5_DDR              DDRD
	#define LED5_BIT              7
	#define LED5_IS_ACTIVE_LOW    0
*/		
		
		
    
    // BUTTONS
    // BTN 0 - PC0 - (pin A0) - Helmet
    #define BTN0_PORT             PORTC
    #define BTN0_PIN              PINC
    #define BTN0_DDR              DDRC
    #define BTN0_BIT              2
    #define BTN0_IS_PRESSED_LOW   0
                               
    // BTN 1 - PC1 - (pin A1) - Eyes/Chest
    #define BTN1_PORT             PORTC
    #define BTN1_PIN              PINC
    #define BTN1_DDR              DDRC
    #define BTN1_BIT              0
    #define BTN1_IS_PRESSED_LOW   0                       
                               
    // BTN 2 - PC2 - (pin A2) - Left hand
    #define BTN2_PORT             PORTC
    #define BTN2_PIN              PINC
    #define BTN2_DDR              DDRC
    #define BTN2_BIT              3
    #define BTN2_IS_PRESSED_LOW   0
    
    // BTN 3 - PC3 - (pin A3) - Right hand
    #define BTN3_PORT             PORTC
    #define BTN3_PIN              PINC
    #define BTN3_DDR              DDRC
    #define BTN3_BIT              1
    #define BTN3_IS_PRESSED_LOW   0
        
       
    // EXT INT
    #define EXTINT0_ENABLED       // PD2 for RF RX signal
    
        
    // ADC
    #define ADC5_ENABLED          // PC5 for battery voltage
           
           
    // PWM
                                      
                               
    // TIMERS
    #define SWTIMERS_MAX          1 // number of timers
    #define TMR_BTN_CHECK         0
    
#endif   // BOARD_IRONMAN_SUIT



// ****************************************************************************
// Global Interrupt control (compiler-dependent)
// ****************************************************************************
#include <avr/interrupt.h>
#define __enable_interrupt()    do { __asm__ __volatile__ ("sei"); } while (0)
#define __disable_interrupt()   do { __asm__ __volatile__ ("cli"); } while (0)
#define __nop()                 do { __asm__ __volatile__ ("nop"); } while (0)


#define BSP_NOP()			   { __nop();}
#define BSP_ALL_INT_ENABLE()   { __enable_interrupt(); } 
#define BSP_ALL_INT_DISABLE()  { __disable_interrupt(); }


// Critical section
// BSP_USE_CRITICAL() Must be placed in function before usage BSP_CRITICAL_BEGIN() and BSP_CRITICAL_END() 
#define BSP_USE_CRITICAL()    unsigned char _critical_irq_flags;  
#define BSP_CRITICAL_BEGIN()  { _critical_irq_flags = SREG; __disable_interrupt(); }
#define BSP_CRITICAL_END()    { SREG = _critical_irq_flags; }

// Statement X in critical section
#define BSP_CRITICAL(x)       { BSP_CRITICAL_BEGIN(); {x;} BSP_CRITICAL_END(); }



#endif  // BSP_H
