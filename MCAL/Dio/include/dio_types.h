/**
 * @file dio_types.h
 * @brief Digital Input/Output driver types and configuration structures
 */

 #ifndef DIO_TYPES_H
 #define DIO_TYPES_H
 
 #include <stdint.h>
 #include "std_types.h"
 
 /**
  * @brief Port identifiers
  */
 typedef enum {
     GPIOA,
     GPIOB,
     GPIOC,
     GPIOD
 } GPIO_t;
 
 /**
  * @brief Pin direction
  */
 typedef enum {
     DIR_INPUT,
     DIR_OUTPUT
 } PinDir_t;
 
 /**
  * @brief Pin level
  */
 typedef enum {
     STATE_LOW,
     STATE_HIGH
 } PinState_t;
 
 /**
  * @brief Configuration for a single pin
  */
 typedef struct {
    GPIO_t enuGPIO;
    uint8_t u8Pin;
    PinDir_t enuPinDir;
 } Dio_Cfg_t;
 
 #endif /* DIO_TYPES_H */
 