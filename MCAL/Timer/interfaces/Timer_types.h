/**
 * @file Timer_types.h
 * @brief Timer driver types and configuration structures
 */

 #ifndef TIMER_TYPES_H
 #define TIMER_TYPES_H
 
 #include <stdint.h>
 #include "std_types.h"
 
 /**
  * @brief Timer channel numbers
  */
typedef enum {
    TIMER_CH0,
    TIMER_CH1,
    TIMER_CH2
} TimerChan_t;
 
 /**
  * @brief Timer interrupt modes
  */
typedef enum {
    TIMER_INT_DISABLED,
    TIMER_INT_ENABLED
} TimerIntMode_t;
 
 /**
  * @brief Timer prescaler values
  */
typedef enum {
    TIMER_NO_CLOCK,
    TIMER_PRESCALER_NO,
    TIMER_PRESCALER_8,
    TIMER_PRESCALER_32,
    TIMER_PRESCALER_64,
    TIMER_PRESCALER_128,
    TIMER_PRESCALER_256,
    TIMER_PRESCALER_1024
} TimerPre_t;
 
 /**
  * @brief Configuration for a single pin
  */
typedef struct
{
    TimerChan_t enuTimerCH;
    TimerIntMode_t enumTimerIntMode;
    TimerPre_t enuTimerPre;
    void (*CBK_Ptr)(void);
}Timer_cfg_t;
 
 #endif /* TIMER_TYPES_H */
 