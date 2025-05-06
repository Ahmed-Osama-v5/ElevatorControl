#ifndef MODE_MANAGER_GCFG_H
#define MODE_MANAGER_GCFG_H

#include "Std_Types.h"

/* Mode Transition Timeouts (in milliseconds) */
#define cu16MODE_TRANSITION_TIMEOUT     ((uint16_t) 5000U)
#define cu16MODE_ERROR_TIMEOUT          ((uint16_t) 3000U)

/* Operation IDs */
#define cu8OPERATION_DOOR_CONTROL       ((uint8_t) 0U)
#define cu8OPERATION_MOTOR_CONTROL      ((uint8_t) 1U)
#define cu8OPERATION_CALL_HANDLING      ((uint8_t) 2U)
#define cu8OPERATION_LED_CONTROL        ((uint8_t) 3U)
#define cu8OPERATION_COUNT              ((uint8_t) 4U)

/* Error Codes */
#define cu8MODE_ERROR_NONE              ((uint8_t) 0U)
#define cu8MODE_ERROR_INVALID_TRANS     ((uint8_t) 1U)
#define cu8MODE_ERROR_TIMEOUT           ((uint8_t) 2U)
#define cu8MODE_ERROR_HW_FAULT          ((uint8_t) 3U)

/* Operation Modes */
typedef enum {
    MODE_NORMAL,
    MODE_PROGRAM,
    MODE_MAINTENANCE,
    MODE_ERROR
} ModeState_t;

/* Mode Change Result */
typedef enum {
    MODE_CHANGE_OK,
    MODE_CHANGE_INVALID,
    MODE_CHANGE_BLOCKED,
    MODE_CHANGE_ERROR
} ModeChangeResult_t;

/* Mode Manager Status */
typedef struct {
    ModeState_t enuCurrentMode;
    uint16_t u16ModeEntryTime;
    uint8_t u8ErrorCode;
    boolean bIsInitialized;
} ModeStatus_t;


#endif // MODE_MANAGER_GCFG_H
