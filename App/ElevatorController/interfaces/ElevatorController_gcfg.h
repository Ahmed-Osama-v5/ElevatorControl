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

/**
 * @file ElevatorController_gcfg.h
 * @brief Global configuration types for the ElevatorController module.
 *
 * Defines all shared data types (enumerations and structs) that describe
 * the elevator's runtime state and are exported to other modules.
 *
 * @defgroup ElevatorController_gcfg ElevatorController Global Config
 * @ingroup App_ElevatorController
 * @{
 */

#include "Std_Types.h"
#include "SystemConfig.h"
#include "CallHandler_gcfg.h"

/** @} */

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of CallHandler configuration settings                             */
/* ************************************************************************ */
/* ************************************************************************ */

/**
 * @brief Specifies how the elevator door is controlled.
 */
typedef enum
{
    DOOR_MANUAL,    /**< Door is operated manually. */
    DOOR_AUTOMATIC  /**< Door is operated automatically by the controller. */
}DoorType_t;

/**
 * @brief Represents the current state of the elevator door.
 */
typedef enum
{
    DOOR_CLOSED,   /**< Door is fully closed. */
    DOOR_OPENING,  /**< Door is in the process of opening. */
    DOOR_OPEN,     /**< Door is fully open. */
    DOOR_CLOSING   /**< Door is in the process of closing. */
}DoorState_t;

/**
 * @brief Represents the current movement direction or motion phase of the elevator.
 */
typedef enum
{
    DIR_IDLE,     /**< Elevator is stationary with no pending direction. */
    DIR_UP,       /**< Elevator is moving upward. */
    DIR_DOWN,     /**< Elevator is moving downward. */
    DIR_SLOWING,  /**< Elevator is decelerating before stopping. */
    DIR_STOPPING  /**< Elevator is in the final stopping phase. */
}MoveDirection_t;

/**
 * @brief Represents the operating mode of the elevator controller.
 */
typedef enum
{
    MODE_INIT,        /**< Initialization mode at power-on. */
    MODE_NORMAL,      /**< Normal passenger service mode. */
    MODE_PROGRAM,     /**< Programming/configuration mode. */
    MODE_MAINTENANCE, /**< Maintenance or inspection mode. */
    MODE_ERROR        /**< Error / fault mode. */
}OperatingMode_t;

/**
 * @brief Specifies the collection direction for call servicing.
 */
typedef enum
{
    COLLECTION_UP,   /**< Collect calls while travelling upward. */
    COLLECTION_DOWN  /**< Collect calls while travelling downward. */
}CollDir_t;

/**
 * @brief Holds the full runtime state of the elevator.
 */
typedef struct
{
    uint8_t u8CurrentFloor;       /**< Current floor position of the elevator cabin. */
    uint8_t u8DestinationFloor;   /**< Target floor the elevator is moving toward. */
    uint8_t u8SlowTimer;          /**< Deceleration timer (configurable, in task ticks). */
    uint8_t u8FastTimer;          /**< High-speed travel timer (configurable, in task ticks). */
    uint8_t u8CamTimer;           /**< CAM pulse timer value. */
    uint8_t u8StopTimer;          /**< Timer used during the stop phase. */
    uint8_t u8LightTimer;         /**< Cabin light auto-off timer (in task ticks). */
    uint8_t u8FloorCount;         /**< Total number of floors in service. */
    uint8_t u8MntSpeed;           /**< Maintenance mode travel speed setting. */
    uint8_t u8ParkFloor;          /**< Parking floor where the elevator returns when idle. */
    uint8_t u8CamFailCnt;         /**< Count of consecutive CAM sensor failures. */
    uint8_t u8CabinPwd;           /**< Cabin-panel access password. */
    uint8_t u8PhsSeq;             /**< Phase sequence configuration byte. */
    uint8_t u8CollectionDir;      /**< Raw collection direction byte. */
    MoveDirection_t enuDirection;             /**< Current movement direction state. */
    MoveDirection_t enuLastTravelDir;         /**< only ever DIR_UP or DIR_DOWN. */
    CollDir_t enuCollDir;                     /**< Active call-collection direction. */
    DoorType_t enuDoorType;                   /**< Configured door type (manual or automatic). */
    DoorState_t enuDoorState;                 /**< Current door state. */
    OperatingMode_t enuOperatingMode;         /**< Active operating mode of the controller. */
    CallType_t aenuFloorCalls[cu8MAX_FLOORS]; /**< Array of pending call types indexed by floor. */
    uint16_t u16DoorTimer;        /**< Door open timer countdown (in milliseconds). */
    boolean bEmergencyStop;       /**< Flag indicating an active emergency stop condition. */
} Elevator_t;

#endif // ELEVATOR_CONTROLLER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
