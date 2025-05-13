/**
 * @file dio.h
 * @brief Digital Input/Output driver header
 */

#ifndef DIO_H
#define DIO_H

#include <avr/io.h>
#include "dio_types.h"

/**
 * @brief Initialize the DIO driver
 * @param Config Pointer to configuration structure
 */
void DIO_Init(const Dio_Cfg_t* Config);

/**
 * @brief Write a value to a pin
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param u8Pin Pin number (0-7)
 * @param enuPinState Pin State (STATE_LOW or STATE_HIGH)
 */
void DIO_WritePin(GPIO_t port, uint8_t u8Pin, PinState_t enuPinState);

/**
 * @brief Read a pin value
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param u8Pin Pin number (0-7)
 * @return Pin State (STATE_LOW or STATE_HIGH)
 */
PinState_t DIO_ReadPin(GPIO_t port, uint8_t u8Pin);

/**
 * @brief Write a value to an entire port
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param value Port value (8-bit)
 */
void DIO_WritePort(GPIO_t port, uint8_t value);

/**
 * @brief Read an entire port value
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @return Port value (8-bit)
 */
uint8_t DIO_ReadPort(GPIO_t port);


 /**
  * @brief Toggles the state of a pin
  * 
  * @param GPIO 
  * @param pins 
  */
  void DIO_Toggle (GPIO_t port, uint8_t u8Pin);

#endif /* DIO_H */
