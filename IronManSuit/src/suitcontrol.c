// ****************************************************************************
// IronManSuit application
//
// Read buttons, toggle LEDs, play sounds, move servos, measure battery, sleep if possible
// ****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <util/atomic.h>

#include "bsp.h"
#include "bsp_gpio.h"
#include "bsp_sleep.h"
#include "bsp_timers.h"
#include "bsp_trace.h"
#include "suitcontrol.h" 





// ****************************************************************************
// Button state
// ****************************************************************************
typedef enum {
    BTN_NO_EVENT,      // 
    BTN_WAIT_RELEASE,  // button was pressed but was not released yet
    BTN_SHORT_CLICK,   // click is waiting to be processed
    BTN_LONG_CLICK     // long click is waiting to be processed
} button_event_t;


typedef struct button_struct_s {
    bool             is_pressed;  // physical state
    button_event_t   event;       // event to be processed
    uint16_t         counter;     // counter for  
} button_struct_t;


// Buttons:
// 0 - helmet
// 1 - eyes/chest
// 2 - left
// 3 - right
static button_struct_t buttons[4];



// ****************************************************************************
// Events
// ****************************************************************************
bool helmet_move;   // Open/close

bool eyes_toggle;  // Just toggle eye leds 
bool chest_toggle;  // Just toggle chest leds 

bool left_toggle;   // Just toggle left hand LED
bool left_effect;   // Toggle left hand LED with effect

bool right_toggle;   // Just toggle right hand LED
bool right_effect;   // Toggle right hand LED with effect






// ****************************************************************************
// Read and process buttons
// ****************************************************************************
// INTERRUPT CONTEXT
// Flag for ISR context - One of buttons was pressed 
static volatile bool button_pressed_isr;

// RF signal handler
// External interrupt INT0 occurs every time receiver gets stable signal.
void EXT_INT0_isr_handler() 
{   
    button_pressed_isr = true;
}



// Check if button was pressed
// Number of the channel can be determined by reading receiver's pins (D0..D3)
// If button is pressed - start timer and wait release
void checkPressed() 
{

    BSP_USE_CRITICAL();
    uint8_t button_pressed_isr_local;
    BSP_CRITICAL(button_pressed_isr_local = button_pressed_isr);
        
    // There was no ISR (there are pressed buttons)    
    if(!button_pressed_isr_local) {
        return;
    }        
    
    for (uint8_t i = 0; i < 4; ++i) {
        if ( ((i == 0) && BSP_BTN0_IS_PRESSED()) ||
             ((i == 1) && BSP_BTN1_IS_PRESSED()) ||
             ((i == 2) && BSP_BTN2_IS_PRESSED()) ||
             ((i == 3) && BSP_BTN3_IS_PRESSED()) )    {
            BSP_timer_start_ms(TMR_BTN_CHECK, SUIT_BUTTON_CHECK_MS, SWTIMER_PERIODIC, checkReleased);
            BSP_TRACE("Button %d pressed", i);            
            buttons[i].is_pressed = true;
            buttons[i].event = BTN_WAIT_RELEASE;
        }
    }
    
    BSP_CRITICAL(button_pressed_isr = false;);
}



// Called when check button timer (TMR_BTN_CHECK) is fired
// Check if button was pressed earlier and released just now
void checkReleased() 
{   
    for (uint8_t i = 0; i < 4; ++i) {
        if (buttons[i].event != BTN_WAIT_RELEASE) {
            continue;
        }
                    
        if ( ((i == 0) && (! BSP_BTN0_IS_PRESSED())) ||
             ((i == 1) && (! BSP_BTN1_IS_PRESSED())) ||
             ((i == 2) && (! BSP_BTN2_IS_PRESSED())) ||
             ((i == 3) && (! BSP_BTN3_IS_PRESSED())) )    {
            BSP_timer_stop(TMR_BTN_CHECK);
            BSP_TRACE("Button released", 0);
            buttons[i].is_pressed = false;
            buttons[i].counter = 0;
            buttons[i].event = BTN_SHORT_CLICK;  // short click detected
        }
        else if (buttons[i].counter * SUIT_BUTTON_CHECK_MS >= SUIT_BUTTON_LONG_MS) {
            BSP_timer_stop(TMR_BTN_CHECK);
            BSP_TRACE("Button timeout (counter %d)", buttons[0].counter);
            buttons[i].is_pressed = false;
            buttons[i].counter = 0;
            buttons[i].event = BTN_LONG_CLICK;   // time is over - long click detected
        }
        else {
            buttons[i].counter++;
        }
    }   
}




void processButtonEvent()
{
    
    if (buttons[0].event == BTN_SHORT_CLICK) {
        buttons[0].event = BTN_NO_EVENT;
        BSP_TRACE("Helmet short click", 0);
        helmet_move = true;
    }
    else if (buttons[0].event == BTN_LONG_CLICK) {
        buttons[0].event = BTN_NO_EVENT;
        BSP_TRACE("Helmet long click", 0);
    }   

    if (buttons[1].event == BTN_SHORT_CLICK) {
        buttons[1].event = BTN_NO_EVENT;
        BSP_TRACE("Eyes/Chest short click", 0);
        eyes_toggle = true;
    }
    else if (buttons[1].event == BTN_LONG_CLICK) {
        buttons[1].event = BTN_NO_EVENT;
        BSP_TRACE("Eyes/Chest long click", 0);
        chest_toggle = true;
    }
     
    if (buttons[2].event == BTN_SHORT_CLICK) {
        buttons[2].event = BTN_NO_EVENT;
        BSP_TRACE("Left hand short click", 0);
        left_toggle = true;
    }
    else if (buttons[2].event == BTN_LONG_CLICK) {
        buttons[2].event = BTN_NO_EVENT;
        BSP_TRACE("Left hand long click", 0);
        left_effect = true;
    }
         
    if (buttons[3].event == BTN_SHORT_CLICK) {
        buttons[3].event = BTN_NO_EVENT;
        BSP_TRACE("Right hand short click", 0);
        right_toggle = true;
    }
    else if (buttons[3].event == BTN_LONG_CLICK) {
        buttons[3].event = BTN_NO_EVENT;
        BSP_TRACE("Right hand long click", 0);
        right_effect = true;
    }

}




// ****************************************************************************
// Change LEDs state
// ****************************************************************************
void processLeds()
{
    
}

// ****************************************************************************
// Servo control with PWM
// ****************************************************************************
void processServos()
{
    
}







// ****************************************************************************
// LEDs on/off with fading
// ****************************************************************************

// from 1 to 3000ms with 1ms step  
void delayMs(uint16_t ms) 
{  
    if (ms) {    
        for (uint16_t i; i < ms; ++i ) {
            _delay_ms(1);
        }
    }    
}  

// from 100 to 1000us with 100us step 
void delayUs_x100(uint16_t us_x100)
{
    if (us_x100) {
        for (uint16_t i; i < us_x100; ++i ) {
            _delay_us(100);
        }
    }
}

#define FADE_STEP_MS      10UL
#define FADE_GRADATIONS   32UL
#define FADE_GRADATION_US FADE_STEP_MS*1000 / FADE_GRADATIONS

static const uint16_t pwmtable_us[FADE_GRADATIONS] =
{
    1,   1,  2,  2,  3,  3,  4,  5,  6,  8, 
    10,  11, 13, 15, 17, 20, 23, 28, 31, 34, 
    36, 39, 43, 47, 51, 56, 61, 67, 75, 81,  
    88, 96
};

 
static void ledFadeOn(uint8_t led_number, uint16_t time_ms)
{
    if (time_ms >= 50) {
        
        uint16_t steps = time_ms / FADE_STEP_MS;
        uint16_t same_steps = steps / FADE_GRADATIONS;
        
        
        for (uint8_t i = 0; i < FADE_GRADATIONS; ++i) {
            for (uint8_t j = 0; j < same_steps; ++j) {
                
                switch (led_number) {
                    case 0: BSP_LED0_ON(); break;
                    case 1: BSP_LED1_ON(); break;
                    case 2: BSP_LED2_ON(); break;
                    case 3: BSP_LED3_ON(); break;
                    case 4: BSP_LED4_ON(); break;
                }
                    
                delayUs_x100(pwmtable_us[i]);
                
                switch (led_number) {
                    case 0: BSP_LED0_OFF(); break;
                    case 1: BSP_LED1_OFF(); break;
                    case 2: BSP_LED2_OFF(); break;
                    case 3: BSP_LED3_OFF(); break;
                    case 4: BSP_LED4_OFF(); break;
                }
                
                delayUs_x100(FADE_STEP_MS*10 - pwmtable_us[i]);      
            }
        }            
    }
    
    switch (led_number) {
        case 0: BSP_LED0_ON(); break;
        case 1: BSP_LED1_ON(); break;
        case 2: BSP_LED2_ON(); break;
        case 3: BSP_LED3_ON(); break;
        case 4: BSP_LED4_ON(); break;
    }
}

static void ledFadeOff(uint8_t led_number, uint16_t time_ms)
{
    if (time_ms >= 50) {
        
        uint16_t steps = time_ms / FADE_STEP_MS;
        uint16_t same_steps = steps / FADE_GRADATIONS;
        
        
        for (uint8_t i = 0; i < FADE_GRADATIONS; ++i) {
            for (uint8_t j = 0; j < same_steps; ++j) {
                
                switch (led_number) {
                    case 0: BSP_LED0_OFF(); break;
                    case 1: BSP_LED1_OFF(); break;
                    case 2: BSP_LED2_OFF(); break;
                    case 3: BSP_LED3_OFF(); break;
                    case 4: BSP_LED4_OFF(); break;
                }
                
                delayUs_x100(pwmtable_us[i]);
                
                switch (led_number) {
                    case 0: BSP_LED0_ON(); break;
                    case 1: BSP_LED1_ON(); break;
                    case 2: BSP_LED2_ON(); break;
                    case 3: BSP_LED3_ON(); break;
                    case 4: BSP_LED4_ON(); break;
                }
                
                delayUs_x100(FADE_STEP_MS*10 - pwmtable_us[i]);   
            }
        }
    }
    
    switch (led_number) {
        case 0: BSP_LED0_OFF(); break;
        case 1: BSP_LED1_OFF(); break;
        case 2: BSP_LED2_OFF(); break;
        case 3: BSP_LED3_OFF(); break;
        case 4: BSP_LED4_OFF(); break;
    }
}





// ****************************************************************************
// Helmet with servos
// ****************************************************************************
static bool helmet_is_open = 1;

static void helmet_toggle() 
{
    uint16_t dst_pwm_us_x100;
    
    BSP_LED4_ON();
    _delay_ms(100);
    
    if (helmet_is_open) {
        dst_pwm_us_x100 = 10; // 1000ms 
    } 
    else {
        dst_pwm_us_x100 = 20; // 2000ms 
    }
    
    
    
    BSP_LED4_OFF();
    _delay_ms(100);
    
}




// ****************************************************************************
// Change effects state
// ****************************************************************************
void processEffects()
{     
    
    if (helmet_move) {      // Helmet open/close
        helmet_move = false;
        BSP_TRACE("Event (helmet_move) processed", 0);    
    }
 
    if (eyes_toggle) {      // Just toggle eye LEDs
        eyes_toggle = false;       
        if (BSP_LED0_IS_ON())  ledFadeOff(0, 500);
        else                   ledFadeOn(0, 500);
        BSP_TRACE("Event (eyes_toggle) processed", 0);
    }
    
    if (chest_toggle) {     // Just toggle chest led
        chest_toggle = false;
        if (BSP_LED1_IS_ON())  ledFadeOff(1, 1500);
        else                   ledFadeOn(1, 1500);
        BSP_TRACE("Event (chest_toggle) processed", 0);
    }
   
    if (left_toggle) {      // Just toggle left hand LED
        left_toggle = false;
        if (BSP_LED2_IS_ON())  ledFadeOff(2, 1000);
        else                   ledFadeOn(2, 1000);
        BSP_TRACE("Event (left_toggle) processed", 0);  
    }

    if (left_effect) {      // Toggle left hand LED with effect
        left_effect = false;
        if (BSP_LED2_IS_ON())  ledFadeOff(2, 1000);
        else                   ledFadeOn(2, 1000);
        BSP_TRACE("Event (left_effect) processed", 0);
    }
    
    if (right_toggle) {      // Just toggle right hand LED
        right_toggle = false;
        if (BSP_LED3_IS_ON())  ledFadeOff(3, 1000);
        else                   ledFadeOn(3, 1000);
        BSP_TRACE("Event (right_toggle) processed", 0); 
    }
       
    if (right_effect) {      // Toggle right hand LED with effect
        right_effect = false;
        if (BSP_LED3_IS_ON())  ledFadeOff(3, 1000);
        else                   ledFadeOn(3, 1000);
        BSP_TRACE("Event (right_effect) processed", 0);
    }

}

// ****************************************************************************
// Battery voltage measurement
// ****************************************************************************
void processBatteryControl()
{
    
}

// ****************************************************************************
// Check state and go to the sleep mode
// ****************************************************************************
void processSleep()
{
    
}


