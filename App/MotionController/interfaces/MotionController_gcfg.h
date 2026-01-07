#ifndef MOTION_CONTROLLER_GCFG_H
#define MOTION_CONTROLLER_GCFG_H

#include "dio_types.h"

/* Relay States */
#define cu8RELAY_ENGAGED         ((PinState_t) STATE_HIGH)
#define cu8RELAY_DISENGAGED      ((PinState_t) STATE_LOW)

/* Motion Control Parameters */
#define cu8MAX_FLOOR             ((uint8_t) 16U)
#define cu16SLOWDOWN_DISTANCE    ((uint16_t) 1U)  /* Floor distance to start slowing */
#define cu16MOTION_TIMEOUT_MS    ((uint16_t) 30000U)

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

/* Motion Commands */
typedef enum {
    MOTION_CMD_STOP,
    MOTION_CMD_MOVE_UP,
    MOTION_CMD_MOVE_DOWN
} MotionCommand_t;


#endif // MOTION_CONTROLLER_GCFG_H
