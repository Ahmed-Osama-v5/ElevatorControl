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

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* cross dependency inclusions ******************************************** */
#include "Std_Types.h"
#include "dio.h"
#include "Timer.h"

/* own header inclusions ************************************************** */

/* export the global configuration */
#include "LEDController_gcfg.h"


/* ************************************************************************ */
/* ************************************************************************ */
/* Exported Functions                                                       */
/* ************************************************************************ */
/* ************************************************************************ */

/* Basic Services ********************************************************* */

/**
 * @brief Initializes the LED Controller module.
 * 
 */
void LEDController_vidInit(void);

/**
 * @brief Sets the state of a specific LED.
 * 
 * @param u8LedId 
 * @param enuState 
 */
void LEDController_vidSetState(uint8_t u8LedId, LEDState_t enuState);

/**
 * @brief Sets the pattern of a specific LED
 * 
 * @param u8LedId 
 * @param enuPattern 
 */
void LEDController_vidSetPattern(uint8_t u8LedId, LEDPattern_t enuPattern);

/**
 * @brief Processes the LED states and patterns
 * 
 */
void LEDController_vidProcess(void);


#endif // LED_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
