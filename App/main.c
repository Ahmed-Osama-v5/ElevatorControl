/**
 * @file main.c
 * @brief Main application file for testAVR
 */

#include <avr/io.h>
#include <util/delay.h>
#include "dio.h"

int main(void)
{
    /* Initialize system */
    DIO_ConfigType dioConfig;
    DIO_Init(&dioConfig);

    while (1)
    {
        /* Toggle PORTA pins */
        PORTA ^= 0xFF; // Toggle all pins on PORTA
        _delay_ms(500); // Delay for 500 ms
    }

    return 0;
}
