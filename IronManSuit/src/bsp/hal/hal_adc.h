// ****************************************************************************
// Hardware access layer for ATmega328p
// ****************************************************************************
// ADC in 8 bit mode with internal reference voltage
//
// To enable ADC, in external file must be defined:
//    ADC0_ENABLED
//    ADC1_ENABLED
//    ADC2_ENABLED
//    ADC3_ENABLED
//    ADC4_ENABLED
//    ADC5_ENABLED
//    BSP_SYS_CLK_HZ
//
// Hardware ADC pins for current MCU:  
//    PC0 - adc0
//    PC1 - adc1
//    PC2 - adc2
//    PC3 - adc3
//    PC4 - adc4
//    PC5 - adc5
// ****************************************************************************
#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <avr/io.h>
#include "bsp.h"
#include "hal/hal_gpio.h"


// ****************************************************************************
// Pin assignment
// ADC is hardware and can't be relocated to another pins:
//    PC0 - adc0
//    PC1 - adc1
//    PC2 - adc2
//    PC3 - adc3
//    PC4 - adc4
//    PC5 - adc5
// **************************************************************************** 
#define ADC0_PORT    PORTC
#define ADC0_DDR     DDRC
#define ADC0_BIT     0

#define ADC1_PORT    PORTC
#define ADC1_DDR     DDRC
#define ADC1_BIT     1

#define ADC2_PORT    PORTC
#define ADC2_DDR     DDRC
#define ADC2_BIT     2

#define ADC3_PORT    PORTC
#define ADC3_DDR     DDRC
#define ADC3_BIT     3

#define ADC4_PORT    PORTC
#define ADC4_DDR     DDRC
#define ADC4_BIT     4

#define ADC5_PORT    PORTC
#define ADC5_DDR     DDRC
#define ADC5_BIT     5


// ****************************************************************************
// Check all pin definitions
// ****************************************************************************
#if ((defined ADC0_ENABLED) && ((defined ADC0_PORT) || !(defined ADC0_DDR) || !(defined ADC0_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif 
#if ((defined ADC1_ENABLED) && (!(defined ADC1_PORT) || !(defined ADC1_DDR) || !(defined ADC1_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif               
#if ((defined ADC2_ENABLED) && (!(defined ADC2_PORT) || !(defined ADC2_DDR) || !(defined ADC2_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif  
#if ((defined ADC3_ENABLED) && (!(defined ADC3_PORT) || !(defined ADC3_DDR) || !(defined ADC3_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif  
#if ((defined ADC4_ENABLED) && (!(defined ADC4_PORT) || !(defined ADC4_DDR) || !(defined ADC4_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif  
#if ((defined ADC5_ENABLED) && (!(defined ADC5_PORT) || !(defined ADC5_DDR) || !(defined ADC5_BIT)))
  #error "ERROR: For ADC0 should be defined 3 params: ADCn_PORT, ADCn_DDR, ADCn_BIT"
#endif  


#if (((defined ADC0_ENABLED) + (defined ADC1_ENABLED) + (defined ADC2_ENABLED) + (defined ADC3_ENABLED) + (defined ADC4_ENABLED) + (defined ADC5_ENABLED)) == 0)
  #warning "WARNING: All ADC are disabled " 
#endif  


// ****************************************************************************
// Macros for initialization
// ****************************************************************************

// Define empty macros at first
#define ADC0_PIN_INIT()   {}
#define ADC1_PIN_INIT()   {}
#define ADC2_PIN_INIT()   {}
#define ADC3_PIN_INIT()   {}
#define ADC4_PIN_INIT()   {}
#define ADC5_PIN_INIT()   {}

// Empty macros will be redefined if current ADC channel is enabled
#ifdef ADC0_ENABLED
    #undef  ADC0_PIN_INIT 
    #define ADC0_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC0_PORT, ADC0_DDR, ADC0_BIT); } 
#endif  

#ifdef ADC1_ENABLED 
    #undef  ADC1_PIN_INIT
    #define ADC1_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC1_PORT, ADC1_DDR, ADC1_BIT); }
#endif 

#ifdef ADC2_ENABLED 
    #undef  ADC2_PIN_INIT
    #define ADC2_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC2_PORT, ADC2_DDR, ADC2_BIT); }
#endif 

#ifdef ADC3_ENABLED 
    #undef  ADC3_PIN_INIT
    #define ADC3_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC3_PORT, ADC3_DDR, ADC3_BIT); }
#endif 

#ifdef ADC4_ENABLED 
    #undef  ADC4_PIN_INIT
    #define ADC4_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC4_PORT, ADC4_DDR, ADC4_BIT); }
#endif

#ifdef ADC5_ENABLED 
    #undef  ADC5_PIN_INIT
    #define ADC5_PIN_INIT()   { HAL_GPIO_INIT_INP_ZZ(ADC5_PORT, ADC5_DDR, ADC5_BIT); }
#endif


//-------------------------------------------------------------------------------
// ADCSRA register: 
//   ADPS2:ADPS0 - prescaler (2 4 8 16 32 64 or 128)
//   ADC clock must be not higher than 200KHz: 
//   16M/128 = 125K  
//   8M/64 = 125K
//   4M/32 = 125K
//   2M/16 = 125K 
//   1M/8 = 125K  
#if (BSP_SYS_CLK_HZ == 16000000)
    #define ADC_SET_PRESCALLER()   { ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); }
#elif (BSP_SYS_CLK_HZ == 8000000)
    #define ADC_SET_PRESCALLER()   { ADCSRA |= (1<<ADPS2)|(1<<ADPS1); }
#elif (BSP_SYS_CLK_HZ == 4000000)
    #define ADC_SET_PRESCALLER()   { ADCSRA |= (1<<ADPS2)|(1<<ADPS0); }
#elif (BSP_SYS_CLK_HZ == 2000000)
    #define ADC_SET_PRESCALLER()   { ADCSRA |= (1<<ADPS2); }     
#elif (BSP_SYS_CLK_HZ == 1000000)
    #define ADC_SET_PRESCALLER()   { ADCSRA |= (1<<ADPS1)|(1<<ADPS0); }
#else 
    #error "ERROR: Missing declaration for BSP_SYS_CLK_HZ (ADC clock)" 
#endif


//-------------------------------------------------------------------------------
// ADMUX register: 
//   REFS1:REFS0 11 - 1.1v reference, ADLAR 1 - Left adjust
// ADCSRB register:
//   ADHSM 0 - no high speed, ISRCEN 0 - AREF pin as analog reference, AREFEN 0 - AREF pin Disable
//   ADTS3:ADTS0 000 - ADC Auto Trigger Not used
// ADCSRA register:
//   ADEN 0 - ADC Disable, ADIE 1 - Interrupt Enable, ADATE 0 - Auto Trigger Disable 
#define ADC_INIT()   {      ADC0_PIN_INIT();                                \
                            ADC1_PIN_INIT();                                \
                            ADC2_PIN_INIT();                                \
                            ADC3_PIN_INIT();                                \
                            ADC4_PIN_INIT();                                \
                            ADC5_PIN_INIT();                                \
	                        ADMUX = (1<<ADLAR) | (1<<REFS1) | (1<<REFS0);   \
					        ADCSRB = 0;                                     \
					        ADCSRA = (1<<ADIE);                             \
					        ADC_SET_PRESCALLER();                               }


// ****************************************************************************
// Macros for init-start-stop-read
// ****************************************************************************   

//-------------------------------------------------------------------------------
// Enable ADC
// ADEN 0    - ADC Disable
// MUX4:MUX0 - select input channel to get measure
// ADEN 1    - ADC Enable
#define ADC_ON(input) { ADCSRA &= ~(1<<ADEN);    \
                        ADMUX |= (input & 0x1F); \
					    ADCSRA |=  (1<<ADEN);       }

// Values of ADMUX (MUX4:MUX0) register
#define ADC_INPUT_0     0x00
#define ADC_INPUT_1     0x01
#define ADC_INPUT_2     0x02
#define ADC_INPUT_3     0x03
#define ADC_INPUT_4     0x04
#define ADC_INPUT_5     0x05
#define ADC_INPUT_TEMP  0x08



//-------------------------------------------------------------------------------
// Disable ADC
// ADEN 0 - ADC Disable
#define ADC_OFF()     { ADCSRA &= ~(1<<ADEN); }

//-------------------------------------------------------------------------------
// Start single convertion
#define ADC_START()   { ADCSRA |= (1<<ADSC); }

//-------------------------------------------------------------------------------
// Last measurement (8bit)
#define ADC_RAW8_GET_VALUE(val)         { val = ADCH; } 

// Last measurement (8bit) and minor bit (0bXXXXXXXX - val, 0b0000000X - bit)
// ADCL must be read first, then ADCH
#define ADC_RAW8_GET_VALUES(val, mbit)  { mbit = ADCL; mbit = ((mbit>>6) & 0x01); val = ADCH;} 

//-------------------------------------------------------------------------------
// Reference
#define BSP_ADC_REF_mV_x10   110

// Convert result to decased of mV
//   mV_x10 = raw8_u16t * ref / 255 
//   Where raw8_u16t - is raw 8-bit adc value (0..255) stroed in 16-bit variable
#define ADC_RAW8_TO_mV_x10(raw8_u16t)  (raw8_u16t * BSP_ADC_REF_mV_x10 / 255) 




#endif  // HAL_ADC_H
