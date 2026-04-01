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

#ifndef MENU_H
#define MENU_H

/**
 * @file Menu.h
 * @brief Public interface for the Menu module.
 *
 * Provides an interactive LCD-based programming menu that allows
 * technicians to view and modify elevator configuration parameters
 * persisted in EEPROM. The menu is active only in @ref MODE_PROGRAM.
 *
 * @defgroup App_Menu Menu
 * @ingroup App
 * @{
 */

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* cross dependency inclusions ******************************************** */


/* own header inclusions ************************************************** */

/* export the global configuration */
#include "Menu_gcfg.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ************************************************************************ */
/* ************************************************************************ */
/* Exported Functions                                                       */
/* ************************************************************************ */
/* ************************************************************************ */

/**
 * @brief Initializes the menu system.
 *
 * Must be called once before any other Menu function. Resets the
 * item list, sets the navigation cursor to the first item, and
 * sets the mode to @ref MENU_MODE_NAVIGATE.
 */
void Menu_Init(void);

/**
 * @brief Periodic task handler for the menu system.
 *
 * Handles timed events such as cursor blinking. Should be called
 * every @ref MENU_BLINK_PERIOD_MS milliseconds.
 */
void Menu_Task(void);

/**
 * @brief Processes user input and updates the menu state.
 *
 * Reads button states to navigate between menu items or increment /
 * decrement the selected item's value while in edit mode.
 * Writes confirmed values to EEPROM.
 */
void Menu_Update(void);

/**
 * @brief Configures a menu item with its label, default value, and bounds.
 *
 * @param itemId         Unique identifier of the menu item (@ref MenuItemId_t).
 * @param label          Null-terminated display label for the item.
 * @param value          Initial (current) value loaded from EEPROM.
 * @param minValue       Minimum allowable value for this parameter.
 * @param maxValue       Maximum allowable value for this parameter.
 * @param eepromAddress  EEPROM address where this parameter is stored.
 */
void Menu_SetItemData(MenuItemId_t itemId, const char* label, uint8_t value, uint8_t minValue, uint8_t maxValue, uint8_t eepromAddress);

/**
 * @brief Updates the stored value of an existing menu item.
 *
 * Refreshes the runtime value without changing the label or bounds.
 * Useful for reloading a parameter from EEPROM after an external write.
 *
 * @param itemId  Unique identifier of the menu item (@ref MenuItemId_t).
 * @param value   New value to assign to the item.
 */
void Menu_UpdateItemValue(MenuItemId_t itemId, uint8_t value);

/** @} */

#ifdef __cplusplus
}
#endif

#endif // MENU_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
