#ifndef SENSOR_MANAGER_LCFG_H
#define SENSOR_MANAGER_LCFG_H

#include "dio_types.h"

#if ! ((defined SensorManager_c) || (defined SensorManager_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* Multiplexer GPIO Configuration */
typedef struct {
    GPIO_t  enuSelectPinA_Port;
    uint8_t u8SelectPinA_Pin;
    GPIO_t  enuSelectPinB_Port;
    uint8_t u8SelectPinB_Pin;
    GPIO_t  enuSelectPinC_Port;
    uint8_t u8SelectPinC_Pin;
    GPIO_t  enuOutput1_Port;    /* SEN_1_OUT */
    uint8_t u8Output1_Pin;
    GPIO_t  enuOutput2_Port;    /* SEN_0_OUT */
    uint8_t u8Output2_Pin;
} MuxConfig_t;

extern const MuxConfig_t cstrMux_Config;

#endif // SENSOR_MANAGER_LCFG_H
