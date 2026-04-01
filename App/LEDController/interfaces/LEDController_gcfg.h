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

#ifndef LED_CONTROLLER_GCFG_H
#define LED_CONTROLLER_GCFG_H

/**
 * @file LEDController_gcfg.h
 * @brief Global configuration types and hardware mappings for the LEDController module.
 *
 * Defines hardware pin assignments, timing constants, and the data types
 * used to control floor indicator LEDs driven by two 74HC573 latches.
 *
 * @defgroup LEDController_gcfg LEDController Global Config
 * @ingroup App_LEDController
 * @{
 */

#include "std_types.h"
#include "SystemConfig.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

/** @name Latch Control Pins
 *  GPIO port and pin assignments for the two 74HC573 latch enable lines.
 *  @{
 */
#define cenuLED_LATCH0_PORT         ((GPIO_t) GPIOB) /**< Port for latch 0 enable (LEDs 0-7). */
#define cu8LED_LATCH0_PIN           ((uint8_t) 2U)   /**< Pin number for latch 0 enable. */
#define cenuLED_LATCH1_PORT         ((GPIO_t) GPIOB) /**< Port for latch 1 enable (LEDs 8-15). */
#define cu8LED_LATCH1_PIN           ((uint8_t) 3U)   /**< Pin number for latch 1 enable. */
/** @} */

/** @name LED Data Port
 *  8-bit parallel data port used to write LED states to the active latch.
 *  @{
 */
#define cenuLED_DATA_PORT           ((GPIO_t) GPIOA) /**< Port carrying the 8-bit LED data bus. */
/** @} */

/**
 * @brief Maps a logical LED index to its physical latch and bit position.
 */
typedef struct {
    uint8_t u8LatchIndex;   /**< Latch index: 0 = first latch (LEDs 0-7), 1 = second latch (LEDs 8-15). */
    uint8_t u8BitPos;       /**< Bit position within the latch (0-7). */
} LEDHWConfig_t;

/** @name LED Pattern Timing
 *  On-time and off-time constants (in milliseconds) for each LED pattern.
 *  @{
 */
#define cu16LED_INTERNAL_CALL_ON_TIME   ((uint16_t) 60000U) /**< Internal call LED on time (ms). */
#define cu16LED_INTERNAL_CALL_OFF_TIME  ((uint16_t) 0U)     /**< Internal call LED off time (ms). */
#define cu16LED_EXTERNAL_CALL_ON_TIME   ((uint16_t) 5000U)  /**< External call LED on time (ms). */
#define cu16LED_EXTERNAL_CALL_OFF_TIME  ((uint16_t) 5000U)  /**< External call LED off time (ms). */
#define cu16LED_MAINTENANCE_ON_TIME     ((uint16_t) 200U)   /**< Maintenance mode LED on time (ms). */
#define cu16LED_MAINTENANCE_OFF_TIME    ((uint16_t) 200U)   /**< Maintenance mode LED off time (ms). */
#define cu16LED_ERROR_ON_TIME           ((uint16_t) 100U)   /**< Error mode LED on time (ms). */
#define cu16LED_ERROR_OFF_TIME          ((uint16_t) 100U)   /**< Error mode LED off time (ms). */
/** @} */

/** @brief Task period for the LED controller (ms). */
#define cu8LED_CONTROLLER_PERIOD_MS   ((uint8_t) 10U)

/**
 * @brief Classifies the call type associated with an LED.
 */
typedef enum {
    LED_CALL_TYPE_INTERNAL, /**< LED corresponds to an internal (cabin) call. */
    LED_CALL_TYPE_EXTERNAL  /**< LED corresponds to an external (landing) call. */
} LEDCallType_t;

/**
 * @brief Represents the instantaneous state of a single LED.
 */
typedef enum {
    LED_STATE_OFF,     /**< LED is off. */
    LED_STATE_ON,      /**< LED is continuously on. */
    LED_STATE_BLINKING /**< LED is blinking according to its assigned pattern. */
} LEDState_t;

/**
 * @brief Pre-defined blink patterns for floor indicator LEDs.
 */
typedef enum {
    LED_PATTERN_NONE,          /**< No active pattern; LED follows its direct state. */
    LED_PATTERN_INTERNAL_CALL, /**< Pattern for a registered internal call. */
    LED_PATTERN_EXTERNAL_CALL, /**< Pattern for a registered external call. */
    LED_PATTERN_MAINTENANCE,   /**< Pattern used during maintenance mode. */
    LED_PATTERN_ERROR          /**< Pattern used during error mode. */
} LEDPattern_t;

/**
 * @brief Configuration entry for a single LED pattern.
 */
typedef struct {
    uint8_t u8LedId;         /**< Logical LED identifier. */
    LEDPattern_t enuPattern; /**< Active blink pattern assigned to this LED. */
    uint16_t u16OnTime;      /**< Duration the LED stays on per cycle (ms). */
    uint16_t u16OffTime;     /**< Duration the LED stays off per cycle (ms). */
} LEDConfig_t;

/**
 * @brief Runtime state of a single floor LED.
 */
typedef struct
{
    boolean bIsActive;              /**< TRUE if the LED has an active call or pattern. */
    LEDCallType_t enuLEDCallType;   /**< Type of call driving this LED. */
    uint16_t u16BlinkTimer;         /**< Blink phase countdown timer (ms). */
    boolean bLEDState;              /**< Current physical on/off output state. */
} FloorLED_t;

/** @brief External declaration of the per-floor hardware configuration array. */
extern const LEDHWConfig_t LED_HWConfig[cu8MAX_FLOORS];

/** @} */

#endif // LED_CONTROLLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
