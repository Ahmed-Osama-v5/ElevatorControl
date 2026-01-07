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

#ifndef CALL_HANDLER_GCFG_H
#define CALL_HANDLER_GCFG_H

#include "std_types.h"
#include "SystemConfig.h"

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

typedef enum
{
    CALL_NONE,
    CALL_INTERNAL,
    CALL_EXTERNAL,
    CALL_BOTH
} CallType_t;

typedef struct
{
    uint8_t u8Floor[cu8MAX_FLOORS];
    CallType_t enuCallType[cu8MAX_FLOORS];
} Call_t;

/*typedef struct
{
    uint8_t u8Floor;         // Floor number (0 to MAX_FLOORS - 1)
    CallType_t enuCallType;    // Type of call (internal, external, or both)
    uint16_t u16Timestamp;     // Timestamp of the call (in milliseconds)
} ElevatorCall_t;

typedef struct {
    ElevatorCall_t strCalls[cu8CALL_QUEUE_SIZE];
    uint8_t u8Head;
    uint8_t u8Tail;
    uint8_t u8Size;
} CallQueue_t;

// LED state tracking
typedef struct {
    boolean bIsBlinking;
    uint16_t u16LastToggle;
    uint8_t u8DutyPercent;
    boolean bCurrentState;
} LedBlinkState_t;
typedef struct {
    CallQueue_t strUpCalls;
    CallQueue_t strDownCalls;
    LedBlinkState_t strLedStates[cu8CALL_QUEUE_SIZE];
    uint16_t u16LastProcessTime;
} CallHandler_t;
*/


#endif // CALL_HANDLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
