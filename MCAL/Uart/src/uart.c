/*
 * uart.c
 *
 *  Created on: Mar 5, 2019
 *      Author: Ahmed
 */


#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include "uart.h"

void UART_init(uint32_t baudRate)
{

uint32_t UBRR_value = lrint((F_CPU /(16L * baudRate)) - 1);

//if(AsyncDoubleSpeed == 1)
//    UCSRA |= 1 << U2X;	// Setting for double speed Asynchronous

// Put the upper part of the baud number here
UBRRH = (unsigned char) (UBRR_value >> 8);

// Put the remaining part of the baud number here
UBRRL = (unsigned char) UBRR_value;

// Enable the receiver and transmitter
UCSRB |= (1 << RXEN) | (1 << TXEN);

UCSRC |= ((1<<URSEL) | (3 << UCSZ0));	// For 8-bit data size
//UCSRC &= ~(1 << USBS); // Setting 1 stop bits

//if(StopBits == 2) UCSRC = (1 << USBS); // Setting 2 stop bits

//if(Parity == EVEN) UCSRC |= 1 << UPM1; // Parity EVEN
//if(Parity == ODD) UCSRC |= 3 << UPM0;	// Parity ODD
//
//if(DataSize == 6) UCSRC |= (1 << UCSZ0);	// For 6-bit data size
//if(DataSize == 7) UCSRC |= (2 << UCSZ0);	// For 7-bit data size
//if(DataSize == 8) UCSRC |= (3 << UCSZ0);	// For 8-bit data size
//if(DataSize == 9) UCSRC |= (7 << UCSZ0);	// For 9-bit data size

}


char UART_Receive_Character(void)
{
	while(! (UCSRA & (1 << RXC)) ); // Waiting for Receiving buffer to be empty
	return UDR;
}

void UART_Receive_String(char str[16])
{
    unsigned char x, i = 0;

    while((x = (char) UART_Receive_Character()) != '\0')
    {
        str[i++] = x;
    }

    str[i] = '\0'; /** Nulling the string **/

}

void UART_Send_String(char *Data)
{
    while(*Data > 0)
        UART_Send_Character(*Data++);
}

void UART_Send_Character(char data)
{
	while (!( UCSRA & (1<<UDRE)));                // wait while register is free
    UDR = data;
}

void UART_Send_Variable(uint8_t var)
{
    char result[10];
    itoa(var,result,10);
    UART_Send_String(result);
}

