#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Std_Types.h"
#include "SensorManager_gcfg.h"

/* Make sure there are no circular dependencies */
#ifdef __cplusplus
extern "C" {
#endif

/* Sensor Status Structure */
typedef struct {
    uint8_t u8SensorState;
    uint8_t u8LastReadStatus;
    uint16_t u16LastReadTime;
} SensorStatus_t;

/* Function Declarations */
extern Std_ReturnType_t SensorManager_stdInit(void);
extern Std_ReturnType_t SensorManager_stdReadSensor(uint8_t u8SensorId, uint8_t* pu8SensorState);
extern Std_ReturnType_t SensorManager_stdReadAllSensors(void);
extern Std_ReturnType_t SensorManager_stdGetSensorStatus(uint8_t u8SensorId, SensorStatus_t* pstrStatus);
extern Std_ReturnType_t SensorManager_stdProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MANAGER_H */