/**
 * @file main.c
 * @brief Main application file for testAVR
 */

#include <avr/io.h>
#include <util/delay.h>
#include "ElevatorController.h"

int main(void)
{
    /* Initialize system */
    ElevatorController_Init();

    while (1)
    {

    }

    return 0;
}
