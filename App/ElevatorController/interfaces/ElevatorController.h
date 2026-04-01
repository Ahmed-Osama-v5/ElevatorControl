/* ************************************************************************ */
/* ************************************************************************ */
/*                        (C) Copyright 2026 by                             */
/*                                 HexaMix                                  */
/*                         All rights reserved                              */
/*           This software is the exclusive property of HexaMix.            */
/*   It cannot be communicated or divulged to anybody without a previous    */
/*  written authorisation.                                                  */
/*                                                                          */
/*     Any partial or complete copy of this program whether it is in its    */
/*   original shape or in its modified shape has to mention this copyright  */
/*  and its proprietor.                                                     */
/*                                                                          */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                        FILE INCLUSION MANAGEMENT                         */
/* ************************************************************************ */
/* ************************************************************************ */
/* Protection against bad and multiple inclusions                           */
/* ************************************************************************ */

#ifndef ELEVATOR_CONTROLLER_H
#define ELEVATOR_CONTROLLER_H

/**
 * @file ElevatorController.h
 * @brief Public interface for the ElevatorController module.
 *
 * Exposes the initialization, operation loops, and service functions
 * of the top-level elevator application controller.
 *
 * @defgroup App_ElevatorController ElevatorController
 * @ingroup App
 * @{
 */

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* own header inclusions ************************************************** */

/* export the global configuration */
#include "ElevatorController_gcfg.h"
#include <stdint.h>


/* ************************************************************************ */
/* ************************************************************************ */
/* Exported Functions                                                       */
/* ************************************************************************ */
/* ************************************************************************ */

/* Basic Services ********************************************************* */

/* Initialise the component. */
/**
* @brief This function initializes and starts the operation loop of the elevator controller.
*/
void ElevatorController_Init(void);

/**
 * @brief Main periodic operation loop of the elevator controller.
 *
 * Must be called repeatedly in normal service mode. Handles call
 * collection, motion control, door management, and LED updates.
 */
void ElevatorController_vidOperationLoop(void);

/**
 * @brief Programming loop of the elevator controller.
 *
 * Executed when the system boots into programming mode. Allows the
 * technician to configure persistent parameters (timers, floor count,
 * door options, etc.) via the on-board menu.
 */
void ElevatorController_vidProgrammingLoop(void);

/* Services ********************************************************* */

/**
 * @brief Retrieves the current floor position of the elevator.
 *
 * @return uint8_t  Zero-based floor index where the cabin is located.
 */
uint8_t ElevatorController_u8GetCurrentFloor(void);

/**
 * @brief Retrieves the selector pole count accumulated since the last reset.
 *
 * The selector pole count is used for floor position tracking via the
 * selector sensor.
 *
 * @return uint8_t  Current selector pole counter value.
 */
uint8_t ElevatorController_u8GetSelectorCnt(void);

/**
 * @brief Starts (or restarts) the cabin light auto-off timer.
 *
 * Call this whenever a cabin door or call button event occurs to
 * keep the cabin light on for the configured duration.
 */
void ElevatorController_vidStartLightTimer(void);

/**
 * @brief Initializes the HAL layer used by the elevator controller.
 *
 * Configures GPIO, timers, and peripheral drivers required by the
 * elevator hardware abstraction layer.
 */
void elevator_hal_vidInit(void);

/**
 * @brief Commands the motor to run in the upward direction.
 */
void elevator_hal_vidMotor_up(void);

/**
 * @brief Commands the motor to run in the downward direction.
 */
void elevator_hal_vidMotor_down(void);

/**
 * @brief Commands the motor to stop.
 */
void elevator_hal_vidMotor_stop(void);

/**
 * @brief Commands the door actuator to open.
 */
void elevator_hal_vidDoor_open(void);

/**
 * @brief Commands the door actuator to close.
 */
void elevator_hal_vidDoor_close(void);

/**
 * @brief Updates the floor indicator display with the given floor number.
 *
 * @param u8Floor  Floor number to show on the display (zero-based index).
 */
void elevator_hal_vidDisplay_floor(uint8_t u8Floor);

/**
 * @brief Updates the status display with a text string.
 *
 * @param cpu8Status  Pointer to a null-terminated status string.
 */
void elevator_hal_vidDisplay_status(const uint8_t* cpu8Status);

/**
 * @brief Starts the HAL millisecond timer.
 */
void elevator_hal_vidTimer_start();

/**
 * @brief Stops the HAL millisecond timer.
 */
void elevator_hal_vidTimer_stop(void);

/**
 * @brief Returns the elapsed time in milliseconds since the timer was started.
 *
 * @return uint16_t  Elapsed time in milliseconds.
 */
uint16_t ElevatorController_u16GetSystemMs(void);

/**
 * @brief Displays the splash screen on the LCD at startup.
 */
void ElevatorController_vidSplashScreen(void);

/**
 * @brief Returns the current operating mode of the elevator controller.
 *
 * @return OperatingMode_t  Active operating mode.
 */
OperatingMode_t ElevatorController_enuGetMode(void);

/** @} */

/* ************************************************************************ */

#endif // ELEVATOR_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
