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

#ifndef LED_CONTROLLER_LCFG_H
#define LED_CONTROLLER_LCFG_H

#if ! ((defined LEDController_c) || (defined LEDController_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* ************************************************************************ */
/* ************************************************************************ */
/*                          CONFIGURATION DATA                              */
/* ************************************************************************ */
/* ************************************************************************ */

/* Private defines */
#define cu8SHIFT_REG_DELAY_US ((uint8_t) 1U)

/* Module private types */
typedef struct {
    LEDState_t enuState;
    LEDPattern_t enuPattern;
    uint16_t u16LastToggleTime;
    boolean bCurrentOutput;
    boolean bIsInitialized;
} LEDStatus_t;

/* LED MUX Hardware Configuration */
const LEDHWConfig_t LED_HWConfig[cu8MAX_FLOORS];

#endif // LED_CONTROLLER_LCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
