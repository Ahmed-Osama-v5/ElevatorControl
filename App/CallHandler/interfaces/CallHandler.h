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

#ifndef CALL_HANDLER_H
#define CALL_HANDLER_H

/**
 * @file CallHandler.h
 * @brief Public interface for the CallHandler module.
 *
 * The CallHandler module scans all floor call inputs (internal cabin
 * buttons and external landing buttons) and registers the resulting
 * calls into the shared @ref Elevator_t state structure.
 *
 * @defgroup App_CallHandler CallHandler
 * @ingroup App
 * @{
 */

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* cross dependency inclusions ******************************************** */
#include "ElevatorController_gcfg.h"
#include "SystemConfig.h"

/* own header inclusions ************************************************** */

/* export the global configuration */
#include "CallHandler_gcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Exported Functions                                                       */
/* ************************************************************************ */
/* ************************************************************************ */

/* Basic Services ********************************************************* */

/* Initialise the component. */
/**
 * @brief Initializes the CallHandler module.
 *
 * Must be called once during system startup before any call scanning
 * is performed. Stores a reference to the shared elevator state.
 *
 * @param pstrElevator  Pointer to the shared @ref Elevator_t state structure.
 */
void CallHandler_vidInit(Elevator_t* pstrElevator);

/**
 * @brief Scans all call inputs and registers active calls.
 *
 * Reads each floor button (internal and external) and updates the
 * @ref Elevator_t::aenuFloorCalls array accordingly. Should be called
 * periodically from the main operation loop.
 */
void CallHandler_vidGetCall(void);

/** @} */

#endif // CALL_HANDLER_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
