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
    Dio_Cfg_t dioConfig;
    dioConfig.enuGPIO = GPIOA; // Set PORTA
    dioConfig.u8Pin = 0; // Set PIN0
    dioConfig.enuPinDir = DIR_OUTPUT; // Set as output
    DIO_Init(&dioConfig); // Initialize DIO

    while (1)
    {
        /* Toggle PORTA pins */
        DIO_Toggle(GPIOA, 0); // Toggle PIN0
        _delay_ms(500); // Delay for 500 ms
    }

    return 0;
}
