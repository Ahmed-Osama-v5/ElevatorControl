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

#ifndef CALL_HANDLER_LCFG_H
#define CALL_HANDLER_LCFG_H

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* other modules of the project ******************************************* */
#include "dio_types.h"
#include <stdint.h>

#if ! ((defined CallHandler_c) || (defined CallHandler_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* ************************************************************************ */
/* ************************************************************************ */
/*                          CONFIGURATION DATA                              */
/* ************************************************************************ */
/* ************************************************************************ */

/* Call inputs GPIO Configuration */
typedef struct {
    GPIO_t  enuCALL_0_Port;
    uint8_t u8CALL_0_Pin;
    GPIO_t  enuCALL_1_Port;
    uint8_t u8CALL_1_Pin;
    GPIO_t  enuCALL_CON_Port;
    uint8_t u8CALL_CON_Pin;
} CallDioConfig_t;

extern const CallDioConfig_t cstrCallDio_Config;

#endif // CALL_HANDLER_LCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
