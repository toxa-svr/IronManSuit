// ****************************************************************************
// UART with FIFO bufferization
//
// To enable UART, in external file must be defined:
//    UART_TX_ENABLED
//    UART_RX_ENABLED
//    UART_BAUD_BPS
//    BSP_SYS_CLK_HZ
//
// ****************************************************************************
#ifndef BSP_UART_H
#define BSP_UART_H

// Processor automatically defined in CodeVisionAVR 
#include <stdint.h>
#include "bsp.h"


// ****************************************************************************
// UART settings
// ****************************************************************************
#define UART_TX_BUFFER_SIZE     256     // size for Tx FIRO UART buffer 
#define UART_RX_BUFFER_SIZE     16      // size for Rx FIRO UART buffer 



// ****************************************************************************
// UART initialization and excahnge
// ****************************************************************************

    // Initialzation software FIFO buffers and hardware UART parts  
    void BSP_uart_init(void);
                            
    // Init UART pins, enable UART clocks, enable UART block itself, enable UART interrupts.
    // Can be used for waking-up after sleep mode. 
    void BSP_uart_enable();
    
    // Disable UART interrupts, disable UART block itself, disable UART clocks, disable UART pins. 
    // Can be used before enabling sleep mode. 
    void BSP_uart_disable();

    // Enqueue the message <data_p> which is of length <len>  
    // TX Interrupt must be enabled 
    void BSP_uart_send(const uint8_t * data_p, uint8_t len);
    
    // Send bytes in loop
    // TX Interrupt must be disabled  
    void BSP_uart_send_no_isr(const uint8_t *data_p, uint8_t len);

    // If RX_CMD_END_SYMBOL was received, function fills the buffer <data> with 
    //   null-terminated string from the receive queue and returns length of 
    //   string (including null-terminator)  
    // Else - function returns zero 
    #define RX_CMD_END_SYMBOL   0x0D // end of uart command (default 0x0D = \r)
    uint8_t BSP_uart_receive (uint8_t* data_p);


#endif
