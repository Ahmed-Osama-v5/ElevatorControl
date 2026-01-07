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
/*
* @brief This function initializes and starts the operation loop of the elevator controller.
*/
void ElevatorController_Init(void);

/* Handler of the component, to be called periodically. */
/*
* @brief This function serves as the main operation loop for the elevator controller,
* managing its periodic tasks.
*/
void ElevatorController_vidOperationLoop(void);

/* Programming loop. */
/*
* @brief This function serves as the programming loop for the elevator controller,
* handling its configuration and setup tasks.
*/
void ElevatorController_vidProgrammingLoop(void);

/* Services ********************************************************* */

/**
 * @brief Retrieves the current floor of the elevator.
 * 
 */
uint8_t ElevatorController_u8GetCurrentFloor(void);

/**
 * @brief Retrieves the selector pole count of the elevator.
 * 
 * @return uint8_t 
 */
uint8_t ElevatorController_u8GetSelectorCnt(void);

// HAL initialization
void elevator_hal_vidInit(void);

// Motor control
void elevator_hal_vidMotor_up(void);
void elevator_hal_vidMotor_down(void);
void elevator_hal_vidMotor_stop(void);

// Door control
void elevator_hal_vidDoor_open(void);
void elevator_hal_vidDoor_close(void);

// Display functions
void elevator_hal_vidDisplay_floor(uint8_t u8Floor);
void elevator_hal_vidDisplay_status(const uint8_t* cpu8Status);

// Timer functions
void elevator_hal_vidTimer_start();
void elevator_hal_vidTimer_stop(void);
uint16_t elevator_hal_u16Get_time_ms(void);


void ElevatorController_vidSplashScreen(void);

OperatingMode_t ElevatorController_enuGetMode(void);

void vidResetDefaults(void);


/* ************************************************************************ */

#endif // ELEVATOR_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
