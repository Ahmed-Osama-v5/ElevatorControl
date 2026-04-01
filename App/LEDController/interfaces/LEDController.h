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

/**
 * @file LEDController.h
 * @brief Public interface for the LEDController module.
 *
 * Manages the state and blink patterns of all floor indicator LEDs
 * connected via two 74HC573 octal latches. Provides functions to
 * set individual LED states, assign blink patterns, and run the
 * periodic LED processing task.
 *
 * @defgroup App_LEDController LEDController
 * @ingroup App
 * @{
 */

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
 * @brief Initializes the LEDController module.
 *
 * Configures the latch and data GPIO lines as outputs, clears all
 * latch outputs, and resets internal LED state tracking.
 */
void LEDController_vidInit(void);

/**
 * @brief Sets the instantaneous state of a specific LED.
 *
 * @param u8LedId   Logical LED index (0 to cu8MAX_FLOORS - 1).
 * @param enuState  Desired state: @ref LED_STATE_OFF, @ref LED_STATE_ON,
 *                  or @ref LED_STATE_BLINKING.
 */
void LEDController_vidSetState(uint8_t u8LedId, LEDState_t enuState);

/**
 * @brief Turns all LEDs off immediately.
 *
 * Writes a cleared state to both latches and resets all internal
 * active flags.
 */
void LEDController_vidTurnAllOff(void);

/**
 * @brief Clears the blink pattern for all LEDs.
 *
 * Sets every LED's pattern to @ref LED_PATTERN_NONE without changing
 * the current physical on/off state.
 */
void LEDController_vidSetPatAllOff(void);

/**
 * @brief Assigns a blink pattern to a specific LED.
 *
 * The pattern determines the on/off timing applied when the LED is
 * processed by @ref LEDController_vidProcess.
 *
 * @param u8LedId      Logical LED index (0 to cu8MAX_FLOORS - 1).
 * @param enuPattern   Blink pattern to assign (see @ref LEDPattern_t).
 */
void LEDController_vidSetPattern(uint8_t u8LedId, LEDPattern_t enuPattern);

/**
 * @brief Periodic processing task for the LED controller.
 *
 * Updates blink timers and writes the resulting LED states to the
 * hardware latches. Must be called every @ref cu8LED_CONTROLLER_PERIOD_MS
 * milliseconds.
 */
void LEDController_vidProcess(void);

/** @} */

#endif // LED_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
