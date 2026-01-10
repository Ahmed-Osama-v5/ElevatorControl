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
 * @brief Initializes the menu system
 * 
 */
void Menu_Init(void);

/**
 * @brief Executes periodic menu tasks
 * 
 */
void Menu_Task(void);

/**
 * @brief Updates the menu state and handles user interactions
 * 
 */
void Menu_Update(void);

/**
 * @brief Sets the data for a menu item
 * 
 * @param itemId The unique identifier for the menu item
 * @param label The text label for the menu item
 * @param value The current value of the menu item
 * @param minValue The minimum allowable value for the menu item
 * @param maxValue The maximum allowable value for the menu item
 * @param eepromAddress The EEPROM address where the menu item data is stored
 */
void Menu_SetItemData(MenuItemId_t itemId, const char* label, uint8_t value, uint8_t minValue, uint8_t maxValue, uint8_t eepromAddress);

/**
 * @brief Updates the value of a menu item
 * 
 * @param itemId The unique identifier for the menu item
 * @param value The new value to set for the menu item
 */
void Menu_UpdateItemValue(MenuItemId_t itemId, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif // MENU_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
