#ifndef CALL_HANDLER_GCFG_H
#define CALL_HANDLER_GCFG_H


#include "SystemConfig.h"

// Call queue configuration
#define cu8CALL_QUEUE_SIZE          cu8MAX_FLOORS

typedef enum
{
    CALL_NONE,
    CALL_INTERNAL,
    CALL_EXTERNAL,
    CALL_BOTH
} CallType_t;

typedef struct
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


#endif // CALL_HANDLER_GCFG_H
