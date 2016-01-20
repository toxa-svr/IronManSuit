// ****************************************************************************
// ADC in 8 bit mode with internal reference voltage
// ****************************************************************************
//
// To enable ADC, in external file must be defined:
//    ADC0_ENABLED
//    ADC1_ENABLED
//    ADC2_ENABLED
//    ADC3_ENABLED
//    ADC4_ENABLED
//    ADC5_ENABLED
//    ADC6_ENABLED
//    ADC7_ENABLED
//    BSP_SYS_CLK_HZ
// ****************************************************************************
#ifndef BSP_ADC_H
#define BSP_ADC_H

#include <stdint.h>
#include "bsp.h"


// ****************************************************************************
// ADC debug
// ****************************************************************************
//#define ADC_DEBUG   // debug output for all ADC measurements


// ****************************************************************************
// ADC init and control
// ****************************************************************************
void BSP_adc_init(void);                   // Init pins, speed, reference
void BSP_adc_enable(uint8_t channel);      // Configure ADC multiplexor with given ADC channel and enable ADC
void BSP_adc_enable_temperature(void);     // Configure ADC multiplexor to measure temperature and enable ADC
void BSP_adc_disable(void);                // Disable ADC
void BSP_adc_start(void);                  // Start single measurement

// Get last measurement
uint8_t BSP_adc_get_last_minor_bit(void);  // Minor bit (most noised) from 10-bit ADC measurement (0b0000000X)
uint8_t BSP_adc_get_last_raw8(void);       // Raw 8-bit ADC measurement (high 8 bits of 10 raw adc bits)
uint8_t BSP_adc_to_mV_x10 (uint8_t raw8);  // Convert raw 8-bit ADC measurement to decades of millivolts






#endif  // BSP_ADC_H
