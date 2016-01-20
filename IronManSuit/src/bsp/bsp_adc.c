// ****************************************************************************
// ADC
// ****************************************************************************
// ADC in 8 bit mode with internal reference voltage
//
// To enable ADC, in external file must be defined:
//    BSP_SYS_CLK_HZ
//
// ****************************************************************************
#include <stdint.h> 
#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_trace.h"
#include "bsp_gpio.h"
#include "bsp_adc.h"


// ADC channel which performed last measurement
static volatile uint8_t adc_chan;

// Raw 8-bit ADC measurement (high 8 bits of 10 raw adc bits)
static volatile uint8_t adc_raw8;

// Lowest bit (noised) of 10 raw adc bits  
static volatile uint8_t adc_raw_minor_bit;



// ****************************************************************************
// Fumctions for init-start-stop-read
// ****************************************************************************
void BSP_adc_init (void) 
{
	// Init all pins, configure speed, reference etc.
	ADC_INIT();
                 
    adc_raw8 = 0;
    adc_raw_minor_bit = 0;
}

//-------------------------------------------------------------------------------
// Configure ADC multiplexor with given ADC channel and enable ADC
void BSP_adc_enable(uint8_t channel)  
{
    switch(channel) {
#ifdef ADC0_ENABLED    
        case 0: ADC_ON(ADC_INPUT_0); break;
#endif
#ifdef ADC1_ENABLED        
        case 1: ADC_ON(ADC_INPUT_1); break;
#endif   
#ifdef ADC2_ENABLED        
        case 2: ADC_ON(ADC_INPUT_2); break;
#endif  
#ifdef ADC3_ENABLED        
        case 3: ADC_ON(ADC_INPUT_3); break;
#endif
#ifdef ADC4_ENABLED        
        case 4: ADC_ON(ADC_INPUT_4); break;
#endif
#ifdef ADC5_ENABLED        
        case 5: ADC_ON(ADC_INPUT_5); break;
#endif
#ifdef ADC6_ENABLED        
        case 6: ADC_ON(ADC_INPUT_6); break;
#endif
#ifdef ADC7_ENABLED        
        case 7: ADC_ON(ADC_INPUT_7); break;
#endif
#ifdef ADC8_ENABLED        
        case 8: ADC_ON(ADC_INPUT_8); break;
#endif       
#ifdef ADC9_ENABLED        
        case 9: ADC_ON(ADC_INPUT_9); break;
#endif
#ifdef ADC10_ENABLED        
        case 10: ADC_ON(ADC_INPUT_10); break;
#endif
        default: BSP_ASSERT(0); break;
    } 
    
    adc_chan = channel;
}

//-------------------------------------------------------------------------------
// Configure ADC multiplexor to measure temperature and enable ADC
void BSP_adc_enable_temperature(void) { ADC_ON(ADC_INPUT_TEMP); }

//-------------------------------------------------------------------------------
// Disable ADC
void BSP_adc_disable(void)            { ADC_OFF();    } 

//-------------------------------------------------------------------------------
// Start single measurement
void BSP_adc_start(void)              { ADC_START();  }



//-------------------------------------------------------------------------------
// Get minor bit (most noised) from 10-bit ADC measurement (0b0000000X)
uint8_t BSP_adc_get_last_minor_bit (void)
{
    uint8_t tmp;  
    BSP_USE_CRITICAL();
    BSP_CRITICAL_BEGIN();
        tmp = adc_raw_minor_bit;   
    BSP_CRITICAL_END();
    
    return tmp;
}

//-------------------------------------------------------------------------------
// Get raw 8-bit ADC measurement (high 8 bits of 10 raw adc bits)
uint8_t BSP_adc_get_last_raw8 (void)
{
    uint8_t tmp;  
    BSP_USE_CRITICAL();
    BSP_CRITICAL_BEGIN();
        tmp = adc_raw8;   
    BSP_CRITICAL_END();
    
    return tmp;
}


//-------------------------------------------------------------------------------
// Convert raw 8-bit ADC measurement to decades of millivolts
uint8_t BSP_adc_to_mV_x10 (uint8_t raw8) 
{       
    uint16_t tmp;  
    tmp = raw8;
    tmp = ADC_RAW8_TO_mV_x10(tmp);

#ifdef ADC_DEBUG 
    BSP_TRACE("ADC%d raw 0x%02X -> %d mV_x10", adc_chan, raw8, tmp);  
#endif

    return (uint8_t)tmp; 
}



//-------------------------------------------------------------------------------
//interrupt  [ADC_INT] void BSP_adcint_isr(void) 
ISR (ADC_vect) { 
    ADC_RAW8_GET_VALUES(adc_raw8, adc_raw_minor_bit);
}    