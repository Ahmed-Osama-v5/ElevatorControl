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

#include "std_types.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

#define MENU_MAX_ITEMS          ((uint8_t) 10U)
#define MENU_BLINK_PERIOD_MS    ((uint16_t) 20U)

typedef enum {
    SLOW_TIMER_MID,
    FAST_TIMER_MID,
    CAM_TIMER__MID,
    STOP_TIMER_MID,
    LIGHT_TIMER_MID,
    DOOR_NUMBER_MID,
    COLLECTION_DIR_MID,
    MNT_SPEED_MID,
    DOOR_OPTIONS_MID,
    PARKING_FLOOR_MID,
    CAM_FAIL_CNT_MID,
    CABIN_PW_MID,
    PHS_SEQ_MID,
    MENU_ITEM_COUNT
} MenuItemId_t;

typedef enum {
    MENU_MODE_NAVIGATE,
    MENU_MODE_EDIT
} MenuMode_t;

typedef struct {
    const char* label;
    uint8_t value;
    uint8_t minValue;
    uint8_t maxValue;
    uint8_t eepromAddress;
} MenuItem_t;

#endif // MENU_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */

