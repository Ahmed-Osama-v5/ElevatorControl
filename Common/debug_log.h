#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifdef DEBUG_
    #include "uart.h"
    #define DBG_PRINT_STRING(str)  do { UART_Send_String(str); UART_Send_Character(13); UART_Send_Character(10);} while(0)
    #define DBG_UART_INIT(baud)  UART_init(baud)
#else
    #define DBG_PRINT_STRING(str)  do { } while(0)
    #define DBG_UART_INIT(baud)  do { } while(0)
#endif

#endif /* DEBUG_LOG_H */
