#ifndef ELEVATOR_CONTROLLER_GCFG_H
#define ELEVATOR_CONTROLLER_GCFG_H

#include "Std_Types.h"
#include "SystemConfig.h"

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
    MoveDirection_t enuDirection;
    DoorType_t enuDoorType;
    DoorState_t enuDoorState;
    OperatingMode_t enuOperatingMode;
    boolean bFloorCalls[cu8MAX_FLOORS];
    //CallType_t enuCallTypes[cu8MAX_FLOORS];
    //FloorLED_t strFloorLEDs[cu8MAX_FLOORS];
    uint16_t u16DoorTimer;
    boolean bEmergencyStop;
} Elevator_t;

#endif // ELEVATOR_CONTROLLER_GCFG_H
