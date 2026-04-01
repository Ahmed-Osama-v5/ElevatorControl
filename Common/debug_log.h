#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#ifdef DEBUG_
    #include "uart.h"
    #define DBG_PRINT_STRING(str)  UART_Send_String(str)
#else
    #define DBG_PRINT_STRING(str)  do { } while(0)
#endif

#endif /* DEBUG_LOG_H */