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

#ifndef ELEVATOR_CONTROLLER_GCFG_H
#define ELEVATOR_CONTROLLER_GCFG_H

#include "Std_Types.h"
#include "SystemConfig.h"


/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

#define DOOR_OPEN_TIME_MS  3000 // Time in milliseconds for door to stay open

typedef enum
{
    DOOR_MANUAL,
    DOOR_AUTOMATIC
}DoorType_t;

typedef enum
{
    DOOR_CLOSED,
    DOOR_OPENING,
    DOOR_OPEN,
    DOOR_CLOSING
}DoorState_t;

typedef enum
{
    DIR_IDLE,
    DIR_UP,
    DIR_DOWN
}MoveDirection_t;

typedef enum
{
    MODE_INIT,
    MODE_NORMAL,
    MODE_PROGRAM,
    MODE_MAINTENANCE,
    MODE_ERROR
}OperatingMode_t;

typedef struct
{
    uint8_t u8CurrentFloor;
    uint8_t u8SlowTimer;
    uint8_t u8FastTimer;
    uint8_t u8CamTimer;
    uint8_t u8StopTimer;
    uint8_t u8LightTimer;
    uint8_t u8FloorCount;
    uint8_t u8MntSpeed;
    uint8_t u8ParkFloor;
    uint8_t u8CamFailCnt;
    uint8_t u8CabinPwd;
    uint8_t u8PhsSeq;
    uint8_t u8CollectionDir;
    MoveDirection_t enuDirection;
    DoorType_t enuDoorType;
    DoorState_t enuDoorState;
    OperatingMode_t enuOperatingMode;
    boolean bFloorCalls[cu8MAX_FLOORS];
    uint16_t u16DoorTimer;
    boolean bEmergencyStop;
} Elevator_t;

#endif // ELEVATOR_CONTROLLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
