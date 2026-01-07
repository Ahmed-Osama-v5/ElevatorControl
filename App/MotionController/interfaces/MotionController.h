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
 * @brief Initialize the Motion Controller.
 */
void MotionController_vidInit(void);

/**
 * @brief Move the elevator in a specific direction.
 * @param enuState The motion state and speed (up, down or stop).
 */
void MotionController_vidMove(MotionState_t enuState);


#endif // MOTION_CONTROLLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
