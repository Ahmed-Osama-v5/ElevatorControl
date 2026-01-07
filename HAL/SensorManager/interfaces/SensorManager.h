#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Std_Types.h"
#include "SystemConfig.h"
#include "SensorManager_gcfg.h"
#include "dio_types.h"

/* Make sure there are no circular dependencies */
#ifdef __cplusplus
extern "C" {
#endif

/* Sensor Status Structure */
typedef struct {
    PinState_t enuSensorState;
    uint8_t u8ActiveCounter;
} SensorStatus_t;

/* Function Declarations */
extern Std_ReturnType_t SensorManager_stdInit(void);
extern Std_ReturnType_t SensorManager_stdReadSensor(Sensor_t enuSensorId, PinState_t* penuSensorState);

inline char* toString(Sensor_t enuSensorId);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MANAGER_H */