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

#include <stdint.h> 
#include "bsp.h"
#include "bsp_hal.h"
#include "bsp_uart.h"
#include "bsp_sleep.h"


// ****************************************************************************
// Check settings
// ****************************************************************************
#if (defined UART_TX_ENABLED) || (defined UART_RX_ENABLED)

    #ifndef UART_BAUD_BPS
        #error "ERROR: No UART_BAUD_BPS definition"   
    #endif
    #ifndef UART_TX_BUFFER_SIZE
        #error "ERROR: No UART_TX_BUFFER_SIZE definition"   
    #endif
    #ifndef UART_RX_BUFFER_SIZE
        #error "ERROR: No UART_RX_BUFFER_SIZE definition"   
    #endif
    
#endif




// ****************************************************************************
// TX and RX buffers
// ****************************************************************************
#ifdef UART_TX_ENABLED
    static uint8_t  tx_buff[UART_TX_BUFFER_SIZE];
    static uint8_t* tx_head = tx_buff;
    static uint8_t* tx_tail = tx_buff;
    static uint8_t  tx_is_going;
    static uint8_t  tx_free_space;
#endif

#ifdef UART_RX_ENABLED
    static uint8_t  rx_buff[UART_RX_BUFFER_SIZE];
    static uint8_t* rx_head = rx_buff;
    static uint8_t* rx_tail = rx_buff;
    static uint8_t  rx_cmd_counter; 
    static uint8_t  rx_free_space;
#endif



// ****************************************************************************
// UART initialization and excahnge
// ****************************************************************************
void BSP_uart_init(void) 
{
    
    // Initialize the buffer pointers
#ifdef UART_TX_ENABLED
    tx_head = tx_buff;
    tx_tail = tx_buff;
    tx_is_going = 0;
    tx_free_space = UART_TX_BUFFER_SIZE - 1;
#endif

#ifdef UART_RX_ENABLED
    rx_head = rx_buff;
    rx_tail = rx_buff;
    rx_cmd_counter = 0; 
    rx_free_space = UART_TX_BUFFER_SIZE - 1;
#endif
      
    // Set buadrate
#if ((defined UART_TX_ENABLED) || (defined UART_RX_ENABLED))            
    UART_SET_BAUD(UART_BAUD_BPS);
#endif

    // Init UART pins, enable UART clocks, enable UART block itself, enable UART interrupts.     
    BSP_uart_enable();
    
}          

//-------------------------------------------------------------------------------
// Init UART pins, enable UART clocks, enable UART block itself, enable UART interrupts.
// Can be used for waking-up after sleep mode. 
void BSP_uart_enable()
{   
    // Init TX and RX pins
#ifdef UART_TX_ENABLED 
    UART_TX_PIN_INIT();
#endif
#ifdef UART_RX_ENABLED
    UART_RX_PIN_INIT();
#endif
    
    // Enable clock for UART
    // Enable UART block itself 
#if ((defined UART_TX_ENABLED) || (defined UART_RX_ENABLED))
    BSP_POWER_UART_ENABLE();            
    UART_ENABLE();
#endif
                   
     // Enable uart in RX and TX modes and enable interrupts
#ifdef UART_TX_ENABLED    
    UART_TX_ENABLE();
    UART_IRQ_TX_ENABLE();
#endif
 
#ifdef UART_RX_ENABLED
    UART_RX_ENABLE();
    UART_IRQ_RX_ENABLE();
#endif
}

//-------------------------------------------------------------------------------
// Disable UART interrupts, disable UART block itself, disable UART clocks, disable UART pins. 
// Can be used before enabling sleep mode. 
void BSP_uart_disable()
{   
    // Disable interrupts and TX and RX modes
#ifdef UART_TX_ENABLED
    UART_IRQ_TX_DISABLE();
    UART_TX_DISABLE();
#endif 

#ifdef UART_RX_ENABLED 
    UART_IRQ_RX_DISABLE(); 
    UART_RX_DISABLE();
#endif    
     
    // Disable UART block itself
    // Disable clock for UART  
#if ((defined UART_TX_ENABLED) || (defined UART_RX_ENABLED))               
    UART_DISABLE();    
    BSP_POWER_UART_DISABLE();
#endif

 
    // Disconnect TX and RX pins
#ifdef UART_TX_ENABLED    
    UART_TX_PIN_DEINIT(); 
#endif    

#ifdef UART_RX_ENABLED  
    UART_RX_PIN_DEINIT();
#endif    
    
} 

// ****************************************************************************
// Transmit functions
// ****************************************************************************
#ifdef UART_TX_ENABLED

void BSP_uart_send(const uint8_t * data_p, uint8_t len)
{
    uint8_t *local_head;
    BSP_USE_CRITICAL(); 
        
    // If there is no data to send
    if ( ! (data_p && len))  return; 
    
    // Check free space in buffer
    BSP_CRITICAL_BEGIN();
        if (tx_free_space <= len) {
            // If there is no space for all bytes - transmit only last bytes
            data_p += len - tx_free_space - 1; 
            len = tx_free_space - 1;
            //BSP_ASSERT_DATA(0, &len, 1); // not enough free space in uart tx buffer
            //return;
        }
    BSP_CRITICAL_END();
             
    
    // Copy all data to the buffer
    BSP_CRITICAL( local_head = tx_head; tx_free_space -= len;);
    if (len) {
        do { 
            if(local_head == tx_buff + UART_TX_BUFFER_SIZE) local_head = tx_buff;
            *local_head++ = *data_p++;    
        } while( --len > 0 ); 
    }                             
    BSP_CRITICAL( tx_head = local_head );  // save new head pointer        
     
    
    // If transfer is not going now - send the first byte manually and correct tail pointer     
    BSP_CRITICAL_BEGIN();
        if (!tx_is_going) {
            tx_is_going = 1;
            if(tx_tail == tx_buff + UART_TX_BUFFER_SIZE) tx_tail = tx_buff; 
            UART_TX(*tx_tail++);
        } 
    BSP_CRITICAL_END();
    
}


//-------------------------------------------------------------------------------
void BSP_uart_send_no_isr(const uint8_t *data_p, uint8_t len)
{
    do{
        UART_TX(*data_p++);
        while (!UART_IRQ_TX_FLAG_IS_UP());
    } while ( --len > 0 );
}


#else 
    // Dummy function, if UART disabled
     void BSP_uart_send(const uint8_t * data_p, uint8_t len) {}
     void BSP_uart_send_no_isr(const uint8_t *data_p, uint8_t len) {}
#endif


// ****************************************************************************
// Receive functions
// ****************************************************************************
#ifdef UART_RX_ENABLED

uint8_t BSP_uart_receive (uint8_t* data_p) 
{
    uint8_t  len = 0; 
    uint8_t *local_tail;
    uint8_t  local_rx_cmd_counter;
    BSP_USE_CRITICAL(); 
    
    BSP_CRITICAL(local_tail = rx_tail; local_rx_cmd_counter = rx_cmd_counter);
    if (local_rx_cmd_counter) {
        do { 
            if(local_tail == rx_buff + UART_RX_BUFFER_SIZE) local_tail = rx_buff;
            *data_p = *local_tail++;
            len++;
            if (*data_p == RX_CMD_END_SYMBOL) {
                *data_p = 0x00;
                break;      
            }
            data_p++;
        } while(len < UART_RX_BUFFER_SIZE);
        
        BSP_CRITICAL(rx_tail = local_tail; rx_cmd_counter--; rx_free_space += len;);        
    }
        
    return len;
}

#else 
    // Dummy function, if UART disabled
    uint8_t BSP_uart_receive (uint8_t* data_p) {return 0;}
#endif // UART_RX_ENABLED




// ****************************************************************************
// Single UART interrupt for TX and RX
// ****************************************************************************
#if (((defined UART_TX_ENABLED) || (defined UART_RX_ENABLED)) && (defined UART_ISR_SINGLE))

interrupt [UART_ISR_VECTOR] void BSP_uart_transfer_complete_isr(void) 
{
#ifdef UART_RX_ENABLED
    uint8_t rx_byte;  
     
    // There was RX interrupt
    if (UART_IRQ_RX_FLAG_IS_UP()) {

        UART_RX(rx_byte);
        
        // End of uart command received
        if (rx_byte == RX_CMD_END_SYMBOL) {
            rx_cmd_counter++;
        }
        // Check free space in buffer
        else if (rx_free_space <= 2) {
            // Return on overflow, but end of command will be always processed
            return;
        }                   
        
        // If wrapping around - reset pointer
        if( rx_head == rx_buff + UART_RX_BUFFER_SIZE) rx_head = rx_buff;
        // Save received byte in buffer
        *rx_head++ = rx_byte;
        rx_free_space--;
    }
#endif // UART_RX_ENABLED
  
#ifdef UART_TX_ENABLED
    // There was TX interrupt
    if (UART_IRQ_TX_FLAG_IS_UP()) {
        
        // If no data in buffer (the last byte is already sent) - stop tx process
        if(tx_head == tx_tail) {
            tx_is_going = 0;
            tx_free_space++;
            // Clear interrupt flag
            UART_IRQ_TX_FLAG_CLR();
            return;
        }    
        
        // If wrapping around - reset pointer
        if(tx_tail == tx_buff + UART_TX_BUFFER_SIZE) tx_tail = tx_buff;
        // Send next one byte from the buffer
        // Interrupt flag will be cleared automatically
        UART_TX(*tx_tail++);
        tx_free_space++;
    }
#endif // UART_TX_ENABLED

    return;
}

#endif // (((defined UART_TX_ENABLED) || (defined UART_RX_ENABLED)) && (defined UART_ISR_SINGLE))

   



// ****************************************************************************
// Separated UART interrupts for TX and RX
// ****************************************************************************
#if ((defined UART_TX_ENABLED) && (defined UART_ISR_TXRX))

//interrupt [UART_ISR_VECTOR_TX] void BSP_uart_tx_complete_isr(void) 
ISR (UART_ISR_VECTOR_TX) 
{ 
    // If no data in buffer (the last byte is already sent) - stop tx process
    if(tx_head == tx_tail) {
        tx_is_going = 0;
        tx_free_space++;
        // Clear interrupt flag
        UART_IRQ_TX_FLAG_CLR();
        return;
    }    
        
    // If wrapping around - reset pointer
    if(tx_tail == tx_buff + UART_TX_BUFFER_SIZE) tx_tail = tx_buff;
    // Send next one byte from the buffer
    // Interrupt flag will be cleared automatically
    UART_TX(*tx_tail++);
    tx_free_space++;
    return;
}
#endif // ((defined UART_TX_ENABLED) && (defined UART_ISR_TXRX))

   


#if ((defined UART_RX_ENABLED) && (defined UART_ISR_TXRX))

//interrupt [UART_ISR_VECTOR_RX] void BSP_uart_rx_complete_isr(void) 
ISR (UART_ISR_VECTOR_RX)
{
    uint8_t rx_byte;   
    // Get a new byte
    UART_RX(rx_byte);
    
    // End of uart command received
    if (rx_byte == RX_CMD_END_SYMBOL) {
        rx_cmd_counter++;
    }
    // Check free space in buffer
    else if (rx_free_space <= 2) {
        // Return on overflow, but end of command will be always processed
        return;
    }                   
        
    // If wrapping around - reset pointer
    if( rx_head == rx_buff + UART_RX_BUFFER_SIZE) rx_head = rx_buff;
    // Save received byte in buffer
    *rx_head++ = rx_byte;
    rx_free_space--;
    return;
}
#endif // ((defined UART_RX_ENABLED) && (defined UART_ISR_TXRX))