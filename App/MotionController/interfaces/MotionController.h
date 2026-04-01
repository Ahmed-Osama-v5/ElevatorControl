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

#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

/**
 * @file MotionController.h
 * @brief Public interface for the MotionController module.
 *
 * Controls the elevator's drive system by activating the appropriate
 * relay outputs for up/down high-speed and low-speed travel, as well
 * as the controlled stop sequence.
 *
 * @defgroup App_MotionController MotionController
 * @ingroup App
 * @{
 */

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* cross dependency inclusions ******************************************** */

#include "Std_Types.h"

/* own header inclusions ************************************************** */

/* export the global configuration */
#include "MotionController_gcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Exported Functions                                                       */
/* ************************************************************************ */
/* ************************************************************************ */

/* Basic Services ********************************************************* */
/**
 * @brief Initializes the MotionController module.
 *
 * Configures the relay output lines and sets the drive to the
 * @ref MOTION_STATE_IDLE state.
 */
void MotionController_vidInit(void);

/**
 * @brief Applies a motion command to the elevator drive.
 *
 * Activates the relay combination that corresponds to the requested
 * @ref MotionState_t. Only one motion state is active at a time;
 * calling this function while the drive is running will switch states
 * immediately.
 *
 * @param enuState  Target motion state (see @ref MotionState_t).
 */
void MotionController_vidMove(MotionState_t enuState);

/** @} */

#endif // MOTION_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
