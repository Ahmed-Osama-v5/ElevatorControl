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

#ifndef MENU_GCFG_H
#define MENU_GCFG_H

/**
 * @file Menu_gcfg.h
 * @brief Global configuration types for the Menu module.
 *
 * Defines the menu item identifiers, operating modes, and the
 * @ref MenuItem_t data structure used by the programming menu system.
 *
 * @defgroup Menu_gcfg Menu Global Config
 * @ingroup App_Menu
 * @{
 */

#include "std_types.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

/**
 * @brief Maximum number of menu items allowed in the system.
 * 
 */
#define MENU_MAX_ITEMS          ((uint8_t) 10U)
/** @brief Blink period for the selected menu item cursor (ms). */
#define MENU_BLINK_PERIOD_MS    ((uint16_t) 20U)

/**
 * @brief Unique identifiers for each configurable menu item.
 */
typedef enum {
    SLOW_TIMER_MID,      /**< Deceleration timer configuration item. */
    FAST_TIMER_MID,      /**< High-speed travel timer configuration item. */
    CAM_TIMER__MID,      /**< CAM pulse timer configuration item. */
    STOP_TIMER_MID,      /**< Stop phase timer configuration item. */
    LIGHT_TIMER_MID,     /**< Cabin light auto-off timer configuration item. */
    DOOR_NUMBER_MID,     /**< Door count / door number configuration item. */
    COLLECTION_DIR_MID,  /**< Call collection direction configuration item. */
    MNT_SPEED_MID,       /**< Maintenance speed configuration item. */
    DOOR_OPTIONS_MID,    /**< Door type options configuration item. */
    PARKING_FLOOR_MID,   /**< Parking floor configuration item. */
    CAM_FAIL_CNT_MID,    /**< CAM failure count threshold configuration item. */
    CABIN_PW_MID,        /**< Cabin panel access password configuration item. */
    PHS_SEQ_MID,         /**< Phase sequence configuration item. */
    MENU_ITEM_COUNT      /**< Total number of menu items (not a valid item ID). */
} MenuItemId_t;

/**
 * @brief Operating mode of the menu navigation state machine.
 */
typedef enum {
    MENU_MODE_NAVIGATE, /**< User is scrolling through menu items. */
    MENU_MODE_EDIT      /**< User is editing the value of the selected item. */
} MenuMode_t;

/**
 * @brief Data record for a single configurable menu item.
 */
typedef struct {
    const char* label;     /**< Display label shown on the LCD for this item. */
    uint8_t value;         /**< Current value of the parameter. */
    uint8_t minValue;      /**< Minimum allowable value. */
    uint8_t maxValue;      /**< Maximum allowable value. */
    uint8_t eepromAddress; /**< EEPROM address where this parameter is persisted. */
} MenuItem_t;

/** @} */

#endif // MENU_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */

