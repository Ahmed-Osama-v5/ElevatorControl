#ifndef SENSOR_MANAGER_GCFG_H
#define SENSOR_MANAGER_GCFG_H

#include "Std_Types.h"
#include "dio_types.h"

/* Sensor Group 1 (Connected to U12 74LS151 - SEN_1_OUT) */
#define cu8SENSOR_FLOOR_M         ((uint8_t) 0U)
#define cu8SENSOR_SELECTOR        ((uint8_t) 1U)
#define cu8SENSOR_STOP_SEN        ((uint8_t) 2U)
#define cu8SENSOR_SHK             ((uint8_t) 3U)
#define cu8SENSOR_LOCK            ((uint8_t) 4U)
#define cu8SENSOR_UP_LIMIT        ((uint8_t) 5U)
#define cu8SENSOR_DN_LIMIT        ((uint8_t) 6U)
#define cu8SENSOR_MNT_SEN         ((uint8_t) 7U)

/* Sensor Group 2 (Connected to U11 74LS151 - SEN_0_OUT) */
#define cu8SENSOR_MNT_UP          ((uint8_t) 8U)
#define cu8SENSOR_MNT_DN          ((uint8_t) 9U)
#define cu8SENSOR_OPEN_DOOR       ((uint8_t) 10U)
#define cu8SENSOR_OVER_LOAD       ((uint8_t) 11U)
#define cu8SENSOR_CLOSE_DOOR      ((uint8_t) 12U)
#define cu8SENSOR_FL              ((uint8_t) 13U)
#define cu8SENSOR_PHASE_1         ((uint8_t) 14U)
#define cu8SENSOR_PHASE_2         ((uint8_t) 15U)

/* Total number of sensors */
#define cu8SENSOR_COUNT           ((uint8_t) 16U)

/* Multiplexer Select Pins (A, B, C inputs of 74LS151) */
#define cu8MUX_SELECT_A           ((uint8_t) 0U)
#define cu8MUX_SELECT_B           ((uint8_t) 1U)
#define cu8MUX_SELECT_C           ((uint8_t) 2U)

/* Multiplexer Output Pins */
/* SEN_1_OUT */
#define cu8MUX1_OUT_PIN          ((uint8_t) 0U)

/* SEN_0_OUT */
#define cu8MUX2_OUT_PIN          ((uint8_t) 1U)


/* Sensor States */
#define cenuSENSOR_STATE_ACTIVE    (STATE_HIGH)
#define cenuSENSOR_STATE_INACTIVE  (STATE_LOW)

#endif /* SENSOR_MANAGER_GCFG_H */