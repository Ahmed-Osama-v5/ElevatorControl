/*
 * uart.h
 *
 *  Created on: Mar 5, 2019
 *      Author: Ahmed
 */

#ifndef UART_H
#define UART_H

#include "std_types.h"

void UART_init(uint32_t baud_Rate);
char UART_Receive_Character(void);
void UART_Receive_String(char str[16]);
void UART_Send_String(char *Data);
void UART_Send_Variable(uint8_t var);
void UART_Send_Character(char data);

#endif /* UART_H */
