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
     DIO_PORTA,
     DIO_PORTB,
     DIO_PORTC,
     DIO_PORTD
 } DIO_PortType;
 
 /**
  * @brief Pin numbers
  */
 typedef uint8_t DIO_PinType;
 
 /**
  * @brief Pin direction
  */
 typedef enum {
     DIO_PIN_INPUT,
     DIO_PIN_OUTPUT
 } DIO_DirectionType;
 
 /**
  * @brief Pin level
  */
 typedef enum {
     DIO_PIN_LOW,
     DIO_PIN_HIGH
 } DIO_LevelType;
 
 /**
  * @brief Configuration for a single pin
  */
 typedef struct {
     DIO_DirectionType direction;
     uint8_t pullUp;     /* 1: Enable pull-up, 0: Disable pull-up */
 } DIO_PinConfigType;
 
 /**
  * @brief Configuration for a single port
  */
 typedef struct {
     DIO_PinConfigType pins[8];
 } DIO_PortConfigType;
 
 /**
  * @brief Complete DIO configuration
  */
 typedef struct {
     DIO_PortConfigType portA;
     DIO_PortConfigType portB;
     DIO_PortConfigType portC;
     DIO_PortConfigType portD;
 } DIO_ConfigType;
 
 /* Configuration Constants */
 #define DIO_DIR_INPUT    ((DIO_DirectionType)DIO_PIN_INPUT)
 #define DIO_DIR_OUTPUT   ((DIO_DirectionType)DIO_PIN_OUTPUT)
 
 #define DIO_PULL_DISABLE 0
 #define DIO_PULL_ENABLE  1
 
 /* Helper macros for configuration */
 #define DIO_PIN_CONFIG(dir, pull) { \
 .direction = (dir), \
                         .pullUp = (pull) \
 }
 
 #endif /* DIO_TYPES_H */
 