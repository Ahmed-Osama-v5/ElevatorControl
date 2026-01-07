/**
 * @file main.c
 * @brief Main application file for testAVR
 */
#ifndef F_CPU
 #define F_CPU	8000000UL
 #endif
#include "ElevatorController.h"


int main(void)
{
    OperatingMode_t enuCurrentMode = MODE_INIT;

    /* Initialize system */
    ElevatorController_Init();

    #if 0
    
    /* Display splash screen */
    ElevatorController_vidSplashScreen();

    /* Get the current operating mode */
    enuCurrentMode = ElevatorController_enuGetMode();

    if(enuCurrentMode == MODE_PROGRAM)
    {
        /* Programming mode loop */
        ElevatorController_vidProgrammingLoop();
    }
    else if (enuCurrentMode == MODE_NORMAL)
    {
        /* Normal operation mode */
        ElevatorController_vidOperationLoop();
    }
    else

    #endif
    {
        /* TODO: Handle this condition */
        while(1);
    }

    return 0;
}