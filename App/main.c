/**
 * @file main.c
 * @brief Application entry point for the HexaMix Elevator Control system.
 *
 * Initializes the elevator controller, displays the startup splash screen,
 * determines the boot operating mode, and dispatches to the appropriate
 * control loop (normal operation or programming).
 *
 * @defgroup App Application Layer
 * @{
 * @defgroup App_main Main
 * @ingroup App
 * @{
 */
#ifndef F_CPU
 #define F_CPU	8000000UL
 #endif
#include "ElevatorController.h"

/**
 * @brief Application entry point.
 *
 * Sequence:
 * -# Initialize all hardware and software sub-modules via
 *    @ref ElevatorController_Init.
 * -# Display the splash screen via @ref ElevatorController_vidSplashScreen.
 * -# Read the boot mode via @ref ElevatorController_enuGetMode.
 * -# If @ref MODE_PROGRAM: enter @ref ElevatorController_vidProgrammingLoop.
 * -# If @ref MODE_NORMAL: enter @ref ElevatorController_vidOperationLoop.
 *
 * @return int  Always returns 0 (never reached on embedded target).
 */
int main(void)
{
    OperatingMode_t enuCurrentMode = MODE_INIT;

    /* Initialize system */
    ElevatorController_Init();
    
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
    {
        
    }
    return 0;
}
/** @} */ /* App_main */
/** @} */ /* App */