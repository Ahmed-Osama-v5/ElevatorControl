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

#ifndef MOTION_CONTROLLER_GCFG_H
#define MOTION_CONTROLLER_GCFG_H

#include "dio_types.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

/* Motion States */
typedef enum {
    MOTION_STATE_IDLE,
    MOTION_STATE_MOVING_UP_HS,    /* Moving Up High Speed */
    MOTION_STATE_MOVING_UP_LS,    /* Moving Up Low Speed */
    MOTION_STATE_MOVING_DOWN_HS,  /* Moving Down High Speed */
    MOTION_STATE_MOVING_DOWN_LS,  /* Moving Down Low Speed */
    MOTION_STATE_STOPPING,        /* Stopping motion */
    MOTION_STATE_ERROR
} MotionState_t;



#endif // MOTION_CONTROLLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
