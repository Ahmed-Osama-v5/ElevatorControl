#ifndef SENSOR_MANAGER_LCFG_H
#define SENSOR_MANAGER_LCFG_H

#include "dio_types.h"
#include <stdint.h>

#if ! ((defined SensorManager_c) || (defined SensorManager_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* Sensor debounce value */
#define cu8SENSOR_DEBOUNCE_COUNTER  ((uint8_t) 1U)

/* Multiplexer GPIO Configuration */
typedef struct {
    GPIO_t  enuSelectPin1_Port;
    uint8_t u8SelectPin1_Pin;
    GPIO_t  enuSelectPin2_Port;
    uint8_t u8SelectPin2_Pin;
    GPIO_t  enuSelectPin3_Port;
    uint8_t u8SelectPin3_Pin;
    GPIO_t  enuOutput0_Port;    /* SEN_0_OUT */
    uint8_t u8Output0_Pin;
    GPIO_t  enuOutput1_Port;    /* SEN_1_OUT */
    uint8_t u8Output1_Pin;
} MuxConfig_t;

extern const MuxConfig_t cstrMux_Config;

#endif // SENSOR_MANAGER_LCFG_H
