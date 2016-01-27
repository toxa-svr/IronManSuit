// ****************************************************************************
// GPIO (special defines for LEDS and BUTTONS)
// ****************************************************************************
//
// For each LEDn (n = 0, 1, 2) ports must be assigned:
//   LEDn_PORT              - PORT register
//   LEDn_PIN               - PIN register
//   LEDn_DDR               - DDR register
//   LEDn_BIT               - bin in all PORT DDR PIN registers
//   LEDn_IS_ACTIVE_LOW     - 1 if led enables on low state of port
//
// For each BTNn (n = 0, 1, 2) ports must be assigned:
//   BTNn_PORT              - PORT register
//   BTNn_PIN               - PIN register
//   BTNn_DDR               - DDR register
//   BTNn_BIT               - bin in all PORT DDR PIN registers
//   BTNn_IS_PRESSED_LOW    - 1 if button pressing implies low state of port 
//
//
// Control macros will be generated:
//   BSP_LEDn_INIT()        - init port as output and turn off LED
//   BSP_LEDn_IS_ON() 
//   BSP_LEDn_ON() 
//   BSP_LEDn_OFF() 
//   BSP_LEDn_TOGGLE() 
//
//   BSP_BTNn_INIT()        - init port as non-pulled input
//   BSP_BTNn_IS_PRESSED()
//
//   LEDS_NUM               - for total number of Leds
//   BTNS_NUM               - for total number of Buttons
//
// ****************************************************************************
#ifndef BSP_GPIO_H
#define BSP_GPIO_H


#include <util/delay.h> 
#include "bsp.h"
#include "bsp_hal.h"

// ****************************************************************************
// Macros generates controls for LEDs ans BUTTONs  
// ****************************************************************************

#define _LED_INIT(port,bit,ddr,pin,low)      { if(low) {HAL_GPIO_INIT_OUT_UP(port,ddr,bit); }else{ HAL_GPIO_INIT_OUT_DN(port,ddr,bit);}}
#define _LED_ON(port,bit,ddr,pin,low)        { if(low) {HAL_GPIO_OUT_DN(port,bit);          }else{ HAL_GPIO_OUT_UP(port,bit);}} 
#define _LED_OFF(port,bit,ddr,pin,low)       { if(low) {HAL_GPIO_OUT_UP(port,bit);          }else{ HAL_GPIO_OUT_DN(port,bit);}}
#define _LED_TOGGLE(port,bit,ddr,pin,low)    { HAL_GPIO_OUT_TOGGLE(port,bit);}
#define _LED_IS_ON(port,bit,ddr,pin,low)     ((low) ? (!HAL_GPIO_IS_UP(pin,bit)) : HAL_GPIO_IS_UP(pin,bit))

#define _BTN_INIT(port,bit,ddr,pin,low)      { HAL_GPIO_INIT_INP_ZZ(port,ddr,bit);}
#define _BTN_IS_PRESSED(port,bit,ddr,pin,low) ((low) ? (!HAL_GPIO_IS_UP(pin,bit)) : HAL_GPIO_IS_UP(pin,bit))


//-------------------------------------------------------------------------------
// Check all params for LED0 are defined
#define _LED0_DEFINES_NUM_ ((defined LED0_PORT)      + \
                            (defined LED0_BIT)       + \
                            (defined LED0_DDR)       + \
                            (defined LED0_PIN)       + \
                            (defined LED0_IS_ACTIVE_LOW))                 
#if (_LED0_DEFINES_NUM_ == 5) 
    // All params are defined. Define control macros for this LED
    #define __LED0_IS_DEFINED
    #define BSP_LED0_ON()     _LED_ON     (LED0_PORT,LED0_BIT,LED0_DDR,LED0_PIN,LED0_IS_ACTIVE_LOW)
    #define BSP_LED0_OFF()    _LED_OFF    (LED0_PORT,LED0_BIT,LED0_DDR,LED0_PIN,LED0_IS_ACTIVE_LOW)
    #define BSP_LED0_TOGGLE() _LED_TOGGLE (LED0_PORT,LED0_BIT,LED0_DDR,LED0_PIN,LED0_IS_ACTIVE_LOW)
    #define BSP_LED0_IS_ON()  _LED_IS_ON  (LED0_PORT,LED0_BIT,LED0_DDR,LED0_PIN,LED0_IS_ACTIVE_LOW)
    #define BSP_LED0_INIT()   _LED_INIT   (LED0_PORT,LED0_BIT,LED0_DDR,LED0_PIN,LED0_IS_ACTIVE_LOW)
#elif (_LED0_DEFINES_NUM_ == 0)
    // LED is not defined 
    #define BSP_LED0_ON()      {}  // no LED
    #define BSP_LED0_OFF()     {}  // no LED
    #define BSP_LED0_TOGGLE()  {}  // no LED
    #define BSP_LED0_IS_ON()   (0) // no LED
    #define BSP_LED0_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 0 should be defined 5 params: LED0_PORT, LED0_BIT, LED0_DDR, LED0_PIN, LED0_IS_ACTIVE_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for LED1 are defined
#define _LED1_DEFINES_NUM_ ((defined LED1_PORT)      + \
                            (defined LED1_BIT)       + \
                            (defined LED1_DDR)       + \
                            (defined LED1_PIN)       + \
                            (defined LED1_IS_ACTIVE_LOW))
#if (_LED1_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this LED
    #define __LED1_IS_DEFINED
    #define BSP_LED1_ON()     _LED_ON     (LED1_PORT,LED1_BIT,LED1_DDR,LED1_PIN,LED1_IS_ACTIVE_LOW)
    #define BSP_LED1_OFF()    _LED_OFF    (LED1_PORT,LED1_BIT,LED1_DDR,LED1_PIN,LED1_IS_ACTIVE_LOW)
    #define BSP_LED1_TOGGLE() _LED_TOGGLE (LED1_PORT,LED1_BIT,LED1_DDR,LED1_PIN,LED1_IS_ACTIVE_LOW)
    #define BSP_LED1_IS_ON()  _LED_IS_ON  (LED1_PORT,LED1_BIT,LED1_DDR,LED1_PIN,LED1_IS_ACTIVE_LOW)
    #define BSP_LED1_INIT()   _LED_INIT   (LED1_PORT,LED1_BIT,LED1_DDR,LED1_PIN,LED1_IS_ACTIVE_LOW)
#elif (_LED1_DEFINES_NUM_ == 0)
    // LED is not defined 
    #define BSP_LED1_ON()      {}  // no LED
    #define BSP_LED1_OFF()     {}  // no LED
    #define BSP_LED1_TOGGLE()  {}  // no LED
    #define BSP_LED1_IS_ON()   (0) // no LED
    #define BSP_LED1_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 1 should be defined 5 params: LED1_PORT, LED1_BIT, LED1_DDR, LED1_PIN, LED1_IS_ACTIVE_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for LED2 are defined
#define _LED2_DEFINES_NUM_ ((defined LED2_PORT)      + \
                            (defined LED2_BIT)       + \
                            (defined LED2_DDR)       + \
                            (defined LED2_PIN)       + \
                            (defined LED2_IS_ACTIVE_LOW))
#if (_LED2_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this LED
    #define __LED2_IS_DEFINED
    #define BSP_LED2_ON()             _LED_ON     (LED2_PORT,LED2_BIT,LED2_DDR,LED2_PIN,LED2_IS_ACTIVE_LOW)
    #define BSP_LED2_OFF()            _LED_OFF    (LED2_PORT,LED2_BIT,LED2_DDR,LED2_PIN,LED2_IS_ACTIVE_LOW)
    #define BSP_LED2_TOGGLE()         _LED_TOGGLE (LED2_PORT,LED2_BIT,LED2_DDR,LED2_PIN,LED2_IS_ACTIVE_LOW)
    #define BSP_LED2_IS_ON()          _LED_IS_ON  (LED2_PORT,LED2_BIT,LED2_DDR,LED2_PIN,LED2_IS_ACTIVE_LOW)
    #define BSP_LED2_INIT()           _LED_INIT   (LED2_PORT,LED2_BIT,LED2_DDR,LED2_PIN,LED2_IS_ACTIVE_LOW)
#elif (_LED2_DEFINES_NUM_ == 0)
    // LED is not defined 
    #define BSP_LED2_ON()      {}  // no LED
    #define BSP_LED2_OFF()     {}  // no LED
    #define BSP_LED2_TOGGLE()  {}  // no LED
    #define BSP_LED2_IS_ON()   (0) // no LED
    #define BSP_LED2_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 2 should be defined 5 params: LED2_PORT, LED2_BIT, LED2_DDR, LED2_PIN, LED2_IS_ACTIVE_LOW"
#endif


//-------------------------------------------------------------------------------
// Check all params for LED3 are defined
#define _LED3_DEFINES_NUM_ ((defined LED3_PORT)      + \
                            (defined LED3_BIT)       + \
                            (defined LED3_DDR)       + \
                            (defined LED3_PIN)       + \
                            (defined LED3_IS_ACTIVE_LOW))
#if (_LED3_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this LED
    #define __LED3_IS_DEFINED
    #define BSP_LED3_ON()             _LED_ON     (LED3_PORT,LED3_BIT,LED3_DDR,LED3_PIN,LED3_IS_ACTIVE_LOW)
    #define BSP_LED3_OFF()            _LED_OFF    (LED3_PORT,LED3_BIT,LED3_DDR,LED3_PIN,LED3_IS_ACTIVE_LOW)
    #define BSP_LED3_TOGGLE()         _LED_TOGGLE (LED3_PORT,LED3_BIT,LED3_DDR,LED3_PIN,LED3_IS_ACTIVE_LOW)
    #define BSP_LED3_IS_ON()          _LED_IS_ON  (LED3_PORT,LED3_BIT,LED3_DDR,LED3_PIN,LED3_IS_ACTIVE_LOW)
    #define BSP_LED3_INIT()           _LED_INIT   (LED3_PORT,LED3_BIT,LED3_DDR,LED3_PIN,LED3_IS_ACTIVE_LOW)
#elif (_LED3_DEFINES_NUM_ == 0)
// LED is not defined
    #define BSP_LED3_ON()      {}  // no LED
    #define BSP_LED3_OFF()     {}  // no LED
    #define BSP_LED3_TOGGLE()  {}  // no LED
    #define BSP_LED3_IS_ON()   (0) // no LED
    #define BSP_LED3_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 3 should be defined 5 params: LED3_PORT, LED3_BIT, LED3_DDR, LED3_PIN, LED3_IS_ACTIVE_LOW"
#endif


//-------------------------------------------------------------------------------
// Check all params for LED4 are defined
#define _LED4_DEFINES_NUM_ ((defined LED4_PORT)      + \
                            (defined LED4_BIT)       + \
                            (defined LED4_DDR)       + \
                            (defined LED4_PIN)       + \
                            (defined LED4_IS_ACTIVE_LOW))
#if (_LED4_DEFINES_NUM_ == 5)
// All params are defined. Define control macros for this LED
#define __LED4_IS_DEFINED
    #define BSP_LED4_ON()             _LED_ON     (LED4_PORT,LED4_BIT,LED4_DDR,LED4_PIN,LED4_IS_ACTIVE_LOW)
    #define BSP_LED4_OFF()            _LED_OFF    (LED4_PORT,LED4_BIT,LED4_DDR,LED4_PIN,LED4_IS_ACTIVE_LOW)
    #define BSP_LED4_TOGGLE()         _LED_TOGGLE (LED4_PORT,LED4_BIT,LED4_DDR,LED4_PIN,LED4_IS_ACTIVE_LOW)
    #define BSP_LED4_IS_ON()          _LED_IS_ON  (LED4_PORT,LED4_BIT,LED4_DDR,LED4_PIN,LED4_IS_ACTIVE_LOW)
    #define BSP_LED4_INIT()           _LED_INIT   (LED4_PORT,LED4_BIT,LED4_DDR,LED4_PIN,LED4_IS_ACTIVE_LOW)
#elif (_LED4_DEFINES_NUM_ == 0)
    // LED is not defined
    #define BSP_LED4_ON()      {}  // no LED
    #define BSP_LED4_OFF()     {}  // no LED
    #define BSP_LED4_TOGGLE()  {}  // no LED
    #define BSP_LED4_IS_ON()   (0) // no LED
    #define BSP_LED4_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 4 should be defined 5 params: LED4_PORT, LED4_BIT, LED4_DDR, LED4_PIN, LED4_IS_ACTIVE_LOW"
#endif


//-------------------------------------------------------------------------------
// Check all params for LED5 are defined
#define _LED5_DEFINES_NUM_ ((defined LED5_PORT)      + \
                            (defined LED5_BIT)       + \
                            (defined LED5_DDR)       + \
                            (defined LED5_PIN)       + \
                            (defined LED5_IS_ACTIVE_LOW))
#if (_LED5_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this LED
    #define __LED5_IS_DEFINED
    #define BSP_LED5_ON()             _LED_ON     (LED5_PORT,LED5_BIT,LED5_DDR,LED5_PIN,LED5_IS_ACTIVE_LOW)
    #define BSP_LED5_OFF()            _LED_OFF    (LED5_PORT,LED5_BIT,LED5_DDR,LED5_PIN,LED5_IS_ACTIVE_LOW)
    #define BSP_LED5_TOGGLE()         _LED_TOGGLE (LED5_PORT,LED5_BIT,LED5_DDR,LED5_PIN,LED5_IS_ACTIVE_LOW)
    #define BSP_LED5_IS_ON()          _LED_IS_ON  (LED5_PORT,LED5_BIT,LED5_DDR,LED5_PIN,LED5_IS_ACTIVE_LOW)
    #define BSP_LED5_INIT()           _LED_INIT   (LED5_PORT,LED5_BIT,LED5_DDR,LED5_PIN,LED5_IS_ACTIVE_LOW)
#elif (_LED5_DEFINES_NUM_ == 0)
    // LED is not defined
    #define BSP_LED5_ON()      {}  // no LED
    #define BSP_LED5_OFF()     {}  // no LED
    #define BSP_LED5_TOGGLE()  {}  // no LED
    #define BSP_LED5_IS_ON()   (0) // no LED
    #define BSP_LED5_INIT()    {}  // no LED
#else
    #error "ERROR: For LED 5 should be defined 5 params: LED5_PORT, LED5_BIT, LED5_DDR, LED5_PIN, LED5_IS_ACTIVE_LOW"
#endif


//-------------------------------------------------------------------------------
// Check all params for LED6 are defined
#define _LED6_DEFINES_NUM_ ((defined LED6_PORT)      + \
							(defined LED6_BIT)       + \
							(defined LED6_DDR)       + \
							(defined LED6_PIN)       + \
							(defined LED6_IS_ACTIVE_LOW))
#if (_LED6_DEFINES_NUM_ == 5)
	// All params are defined. Define control macros for this LED
	#define __LED6_IS_DEFINED
	#define BSP_LED6_ON()             _LED_ON     (LED6_PORT,LED6_BIT,LED6_DDR,LED6_PIN,LED6_IS_ACTIVE_LOW)
	#define BSP_LED6_OFF()            _LED_OFF    (LED6_PORT,LED6_BIT,LED6_DDR,LED6_PIN,LED6_IS_ACTIVE_LOW)
	#define BSP_LED6_TOGGLE()         _LED_TOGGLE (LED6_PORT,LED6_BIT,LED6_DDR,LED6_PIN,LED6_IS_ACTIVE_LOW)
	#define BSP_LED6_IS_ON()          _LED_IS_ON  (LED6_PORT,LED6_BIT,LED6_DDR,LED6_PIN,LED6_IS_ACTIVE_LOW)
	#define BSP_LED6_INIT()           _LED_INIT   (LED6_PORT,LED6_BIT,LED6_DDR,LED6_PIN,LED6_IS_ACTIVE_LOW)
#elif (_LED6_DEFINES_NUM_ == 0)
	// LED is not defined
	#define BSP_LED6_ON()      {}  // no LED
	#define BSP_LED6_OFF()     {}  // no LED
	#define BSP_LED6_TOGGLE()  {}  // no LED
	#define BSP_LED6_IS_ON()   (0) // no LED
	#define BSP_LED6_INIT()    {}  // no LED
#else
	#error "ERROR: For LED 6 should be defined 5 params: LED6_PORT, LED6_BIT, LED6_DDR, LED6_PIN, LED6_IS_ACTIVE_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for LED7 are defined
#define _LED7_DEFINES_NUM_ ((defined LED7_PORT)      + \
							(defined LED7_BIT)       + \
							(defined LED7_DDR)       + \
							(defined LED7_PIN)       + \
							(defined LED7_IS_ACTIVE_LOW))
#if (_LED7_DEFINES_NUM_ == 5)
	// All params are defined. Define control macros for this LED
	#define __LED7_IS_DEFINED
	#define BSP_LED7_ON()             _LED_ON     (LED7_PORT,LED7_BIT,LED7_DDR,LED7_PIN,LED7_IS_ACTIVE_LOW)
	#define BSP_LED7_OFF()            _LED_OFF    (LED7_PORT,LED7_BIT,LED7_DDR,LED7_PIN,LED7_IS_ACTIVE_LOW)
	#define BSP_LED7_TOGGLE()         _LED_TOGGLE (LED7_PORT,LED7_BIT,LED7_DDR,LED7_PIN,LED7_IS_ACTIVE_LOW)
	#define BSP_LED7_IS_ON()          _LED_IS_ON  (LED7_PORT,LED7_BIT,LED7_DDR,LED7_PIN,LED7_IS_ACTIVE_LOW)
	#define BSP_LED7_INIT()           _LED_INIT   (LED7_PORT,LED7_BIT,LED7_DDR,LED7_PIN,LED7_IS_ACTIVE_LOW)
#elif (_LED7_DEFINES_NUM_ == 0)
	// LED is not defined
	#define BSP_LED7_ON()      {}  // no LED
	#define BSP_LED7_OFF()     {}  // no LED
	#define BSP_LED7_TOGGLE()  {}  // no LED
	#define BSP_LED7_IS_ON()   (0) // no LED
	#define BSP_LED7_INIT()    {}  // no LED
#else
	#error "ERROR: For LED 7 should be defined 5 params: LED7_PORT, LED7_BIT, LED7_DDR, LED7_PIN, LED7_IS_ACTIVE_LOW"
#endif


//-------------------------------------------------------------------------------
// Total number of LEDs
#define LEDS_NUM ((defined __LED0_IS_DEFINED) + (defined __LED1_IS_DEFINED) + (defined __LED2_IS_DEFINED) + (defined __LED3_IS_DEFINED) + (defined __LED4_IS_DEFINED) + (defined __LED5_IS_DEFINED) + (defined __LED6_IS_DEFINED) + (defined __LED7_IS_DEFINED))
#if (LEDS_NUM == 0)
    #warning "WARNING: All LEDs are disabled"
#endif

    
// ****************************************************************************
// Check all params for BTN0 are defined
#define _BTN0_DEFINES_NUM_ ((defined BTN0_PORT)           + \
                            (defined BTN0_BIT)            + \
                            (defined BTN0_DDR)            + \
                            (defined BTN0_PIN)            + \
                            (defined BTN0_IS_PRESSED_LOW))
#if (_BTN0_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this BTN
    #define __BTN0_IS_DEFINED
    #define BSP_BTN0_INIT()        _BTN_INIT       (BTN0_PORT,BTN0_BIT,BTN0_DDR,BTN0_PIN,BTN0_IS_PRESSED_LOW)
    #define BSP_BTN0_IS_PRESSED()  _BTN_IS_PRESSED (BTN0_PORT,BTN0_BIT,BTN0_DDR,BTN0_PIN,BTN0_IS_PRESSED_LOW)
#elif (_BTN0_DEFINES_NUM_ == 0)
    // BTN is not defined 
    #define BSP_BTN0_INIT()        {}  // no BTN
    #define BSP_BTN0_IS_PRESSED()  (0) // no BTN
#else
    #error "ERROR: For BTN 0 should be defined 5 params: BTN0_PORT, BTN0_BIT, BTN0_DDR, BTN0_PIN, BTN0_IS_PRESSED_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for BTN1 are defined
#define _BTN1_DEFINES_NUM_ ((defined BTN1_PORT)           + \
                            (defined BTN1_BIT)            + \
                            (defined BTN1_DDR)            + \
                            (defined BTN1_PIN)            + \
                            (defined BTN1_IS_PRESSED_LOW))
#if (_BTN1_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this BTN
    #define __BTN1_IS_DEFINED
    #define BSP_BTN1_INIT()        _BTN_INIT       (BTN1_PORT,BTN1_BIT,BTN1_DDR,BTN1_PIN,BTN1_IS_PRESSED_LOW  )
    #define BSP_BTN1_IS_PRESSED()  _BTN_IS_PRESSED (BTN1_PORT,BTN1_BIT,BTN1_DDR,BTN1_PIN,BTN1_IS_PRESSED_LOW  )
#elif (_BTN1_DEFINES_NUM_ == 0)
    // BTN is not defined 
    #define BSP_BTN1_INIT()        {}  // no BTN
    #define BSP_BTN1_IS_PRESSED()  (0) // no BTN
#else
    #error "ERROR: For BTN 1 should be defined 5 params: BTN1_PORT, BTN1_BIT, BTN1_DDR, BTN1_PIN, BTN1_IS_PRESSED_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for BTN2 are defined
#define _BTN2_DEFINES_NUM_ ((defined BTN2_PORT)           + \
                            (defined BTN2_BIT)            + \
                            (defined BTN2_DDR)            + \
                            (defined BTN2_PIN)            + \
                            (defined BTN2_IS_PRESSED_LOW))
#if (_BTN2_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this BTN
    #define __BTN2_IS_DEFINED
    #define BSP_BTN2_INIT()        _BTN_INIT       (BTN2_PORT,BTN2_BIT,BTN2_DDR,BTN2_PIN,BTN2_IS_PRESSED_LOW)  
    #define BSP_BTN2_IS_PRESSED()  _BTN_IS_PRESSED (BTN2_PORT,BTN2_BIT,BTN2_DDR,BTN2_PIN,BTN2_IS_PRESSED_LOW)
#elif (_BTN2_DEFINES_NUM_ == 0)
    // BTN is not defined 
    #define BSP_BTN2_INIT()        {}  // no BTN
    #define BSP_BTN2_IS_PRESSED()  (0) // no BTN
#else
    #error "ERROR: For BTN 2 should be defined 5 params: BTN2_PORT, BTN2_BIT, BTN2_DDR, BTN2_PIN, BTN2_IS_PRESSED_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for BTN3 are defined
#define _BTN3_DEFINES_NUM_ ((defined BTN3_PORT)           + \
    (defined BTN3_BIT)            + \
    (defined BTN3_DDR)            + \
    (defined BTN3_PIN)            + \
    (defined BTN3_IS_PRESSED_LOW))
#if (_BTN3_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this BTN
    #define __BTN3_IS_DEFINED
    #define BSP_BTN3_INIT()        _BTN_INIT       (BTN3_PORT,BTN3_BIT,BTN3_DDR,BTN3_PIN,BTN3_IS_PRESSED_LOW)
    #define BSP_BTN3_IS_PRESSED()  _BTN_IS_PRESSED (BTN3_PORT,BTN3_BIT,BTN3_DDR,BTN3_PIN,BTN3_IS_PRESSED_LOW)
#elif (_BTN3_DEFINES_NUM_ == 0)
    // BTN is not defined
    #define BSP_BTN3_INIT()        {}  // no BTN
    #define BSP_BTN3_IS_PRESSED()  (0) // no BTN
#else
    #error "ERROR: For BTN 3 should be defined 5 params: BTN3_PORT, BTN3_BIT, BTN3_DDR, BTN3_PIN, BTN3_IS_PRESSED_LOW"
#endif

//-------------------------------------------------------------------------------
// Check all params for BTN4 are defined
#define _BTN4_DEFINES_NUM_ ((defined BTN4_PORT)           + \
                            (defined BTN4_BIT)            + \
                            (defined BTN4_DDR)            + \
                            (defined BTN4_PIN)            + \
                            (defined BTN4_IS_PRESSED_LOW))
#if (_BTN4_DEFINES_NUM_ == 5)
    // All params are defined. Define control macros for this BTN
    #define __BTN4_IS_DEFINED
    #define BSP_BTN4_INIT()        _BTN_INIT       (BTN4_PORT,BTN4_BIT,BTN4_DDR,BTN4_PIN,BTN4_IS_PRESSED_LOW)
    #define BSP_BTN4_IS_PRESSED()  _BTN_IS_PRESSED (BTN4_PORT,BTN4_BIT,BTN4_DDR,BTN4_PIN,BTN4_IS_PRESSED_LOW)
#elif (_BTN4_DEFINES_NUM_ == 0)
    // BTN is not defined
    #define BSP_BTN4_INIT()        {}  // no BTN
    #define BSP_BTN4_IS_PRESSED()  (0) // no BTN
#else
    #error "ERROR: For BTN 4 should be defined 5 params: BTN4_PORT, BTN4_BIT, BTN4_DDR, BTN4_PIN, BTN4_IS_PRESSED_LOW"
#endif

//-------------------------------------------------------------------------------
// Total number of BTNs
#define BTNS_NUM ((defined __BTN0_IS_DEFINED) + (defined __BTN1_IS_DEFINED) + (defined __BTN2_IS_DEFINED) + (defined __BTN3_IS_DEFINED) + (defined __BTN4_IS_DEFINED))
#if (BTNS_NUM == 0)
    #warning "WARNING: All BUTTONS are disabled"
#endif

// *****************************************************************************
// *****************************************************************************
// *****************************************************************************





//-------------------------------------------------------------------------------
// Simultaneous actions

#define BSP_LEDS_INIT()    { BSP_LED0_INIT(); BSP_LED1_INIT(); BSP_LED2_INIT(); BSP_LED3_INIT(); BSP_LED4_INIT(); BSP_LED5_INIT(); BSP_LED6_INIT(); BSP_LED7_INIT();}
#define BSP_BTNS_INIT()    { BSP_BTN0_INIT(); BSP_BTN1_INIT(); BSP_BTN2_INIT(); BSP_BTN3_INIT(); BSP_BTN4_INIT(); } 

#define BSP_LEDS_ON()      { BSP_LED0_ON();     BSP_LED1_ON();     BSP_LED2_ON();     BSP_LED3_ON();     BSP_LED4_ON();     BSP_LED5_ON();     BSP_LED6_ON();     BSP_LED7_ON();}
#define BSP_LEDS_OFF()     { BSP_LED0_OFF();    BSP_LED1_OFF();    BSP_LED2_OFF();    BSP_LED3_OFF();    BSP_LED4_OFF();    BSP_LED5_OFF();    BSP_LED6_OFF();    BSP_LED7_OFF();}
#define BSP_LEDS_TOGGLE()  { BSP_LED0_TOGGLE(); BSP_LED1_TOGGLE(); BSP_LED2_TOGGLE(); BSP_LED3_TOGGLE(); BSP_LED4_TOGGLE(); BSP_LED5_TOGGLE(); BSP_LED6_TOGGLE(); BSP_LED7_TOGGLE();}

#define BSP_BTNS_ARE_PRESSED  ( BSP_BTN0_IS_PRESSED()   &&  BSP_BTN1_IS_PRESSED()   && BSP_BTN2_IS_PRESSED()    && BSP_BTN3_IS_PRESSED()    && BSP_BTN4_IS_PRESSED())
#define BSP_BTNS_ARE_RELEASED ((!BSP_BTN0_IS_PRESSED()) && (!BSP_BTN1_IS_PRESSED()) && (!BSP_BTN2_IS_PRESSED()) && (!BSP_BTN3_IS_PRESSED()) && (!BSP_BTN4_IS_PRESSED()))


//-------------------------------------------------------------------------------
// Two short blinks at startup (1000 ms total)
// __              ______              ______
//   |____300_____|  100 |____200_____|  100 |____300_____
//
#define BSP_LEDS_TEST_BLINK() {     BSP_LEDS_OFF();                      \
                                    _delay_ms(300);BSP_LEDS_TOGGLE();     \
                                    _delay_ms(100);BSP_LEDS_TOGGLE();     \
                                    _delay_ms(200);BSP_LEDS_TOGGLE();     \
                                    _delay_ms(100);                       \
                                    BSP_LEDS_OFF();                      \
                                    _delay_ms(300);                       \
                              }
                              

                           
#endif
