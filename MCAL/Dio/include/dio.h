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
void DIO_Init(const DIO_ConfigType* Config);

/**
 * @brief Write a value to a pin
 * @param port Port ID (PORTA, PORTB, PORTC, PORTD)
 * @param pin Pin number (0-7)
 * @param value Pin value (0 or 1)
 */
void DIO_WritePin(DIO_PortType port, DIO_PinType pin, DIO_LevelType value);

/**
 * @brief Read a pin value
 * @param port Port ID (PORTA, PORTB, PORTC, PORTD)
 * @param pin Pin number (0-7)
 * @return Pin value (0 or 1)
 */
DIO_LevelType DIO_ReadPin(DIO_PortType port, DIO_PinType pin);

/**
 * @brief Write a value to an entire port
 * @param port Port ID (PORTA, PORTB, PORTC, PORTD)
 * @param value Port value (8-bit)
 */
void DIO_WritePort(DIO_PortType port, uint8_t value);

/**
 * @brief Read an entire port value
 * @param port Port ID (PORTA, PORTB, PORTC, PORTD)
 * @return Port value (8-bit)
 */
uint8_t DIO_ReadPort(DIO_PortType port);

#endif /* DIO_H */
