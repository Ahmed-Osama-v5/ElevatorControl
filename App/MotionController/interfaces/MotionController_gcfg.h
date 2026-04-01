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

/**
 * @file MotionController_gcfg.h
 * @brief Global configuration types for the MotionController module.
 *
 * Defines the @ref MotionState_t enumeration that describes all
 * possible motion states of the elevator drive system.
 *
 * @defgroup MotionController_gcfg MotionController Global Config
 * @ingroup App_MotionController
 * @{
 */

#include "dio_types.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

/**
 * @brief Defines the motion states for the elevator drive system.
 * 
 */
/* Motion States */
typedef enum {
    MOTION_STATE_IDLE,           /**< Drive is stopped with no pending command. */
    MOTION_STATE_MOVING_UP_HS,   /**< Moving upward at high speed. */
    MOTION_STATE_MOVING_UP_LS,   /**< Moving upward at low (slow) speed. */
    MOTION_STATE_MOVING_DOWN_HS, /**< Moving downward at high speed. */
    MOTION_STATE_MOVING_DOWN_LS, /**< Moving downward at low (slow) speed. */
    MOTION_STATE_STOPPING,       /**< Drive is executing a controlled stop sequence. */
    MOTION_STATE_ERROR           /**< Drive is in a fault state. */
} MotionState_t;

/** @} */

#endif // MOTION_CONTROLLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
