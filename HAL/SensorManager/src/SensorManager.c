
#include "dio_types.h"

#define SensorManager_c

#include "SensorManager_lcfg.h"
#include "SensorManager.h"
#include "dio.h"
#include "Timer.h"

/* Private Variables */
static SensorStatus_t strSensorStatus[cu8SENSOR_COUNT];
static boolean bModuleInitialized = False;

const MuxConfig_t cstrMux_Config = {
    .enuSelectPinA_Port = GPIOA,
    .u8SelectPinA_Pin = 0,
    .enuSelectPinB_Port = GPIOA,
    .u8SelectPinB_Pin = 1,
    .enuSelectPinC_Port = GPIOA,
    .u8SelectPinC_Pin = 2,
    .enuOutput1_Port = GPIOB,
    .u8Output1_Pin = 0,
    .enuOutput2_Port = GPIOB,
    .u8Output2_Pin = 1
};

/* Private Function Declarations */
static Std_ReturnType_t stdSetMuxChannel(uint8_t u8Channel);
static Std_ReturnType_t stdReadMuxOutput(uint8_t u8MuxNumber, uint8_t* pu8State);
static uint16_t u16GetTimeMs(void);

/* Function Implementations */
Std_ReturnType_t SensorManager_stdInit(void) {
    Std_ReturnType_t stdResult = E_OK;
    uint8_t u8I;
    
    if(bModuleInitialized == True) {
        return E_NOT_OK;
    }

    /* Initialize GPIO for multiplexer select pins */
    Dio_Cfg_t dio_config = {
        .enuGPIO = cstrMux_Config.enuSelectPinA_Port,
        .u8Pin = cstrMux_Config.u8SelectPinA_Pin,
        .enuPinDir = DIR_OUTPUT
    };
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuSelectPinB_Port;
    dio_config.u8Pin = cstrMux_Config.u8SelectPinB_Pin;
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuSelectPinC_Port;
    dio_config.u8Pin = cstrMux_Config.u8SelectPinC_Pin;
    DIO_Init(&dio_config);
    
    /* Initialize GPIO for multiplexer outputs */
    dio_config.enuGPIO = cstrMux_Config.enuOutput1_Port;
    dio_config.u8Pin = cstrMux_Config.u8Output1_Pin;
    dio_config.enuPinDir = DIR_INPUT;
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuOutput2_Port;
    dio_config.u8Pin = cstrMux_Config.u8Output2_Pin;
    DIO_Init(&dio_config);
    
    /* Initialize sensor status array */
    for(u8I = 0; u8I < cu8SENSOR_COUNT; u8I++) {
        strSensorStatus[u8I].u8SensorState = cu8SENSOR_STATE_INACTIVE;
        strSensorStatus[u8I].u8LastReadStatus = cu8SENSOR_READ_OK;
        strSensorStatus[u8I].u16LastReadTime = 0;
    }
    
    bModuleInitialized = True;
    return stdResult;
}

Std_ReturnType_t SensorManager_stdReadSensor(uint8_t u8SensorId, uint8_t* pu8SensorState) {
    Std_ReturnType_t stdResult = E_OK;
    uint8_t u8MuxChannel;
    uint8_t u8MuxNumber;
    uint8_t u8ReadState;
    
    if(!bModuleInitialized || u8SensorId >= cu8SENSOR_COUNT || pu8SensorState == NULL) {
        return E_NOT_OK;
    }
    
    /* Determine which multiplexer and channel to use */
    if(u8SensorId < 8) {
        u8MuxNumber = 1;  /* U12 - SEN_1_OUT */
        u8MuxChannel = u8SensorId;
    } else {
        u8MuxNumber = 2;  /* U11 - SEN_0_OUT */
        u8MuxChannel = u8SensorId - 8;
    }
    
    /* Set multiplexer channel */
    stdResult = stdSetMuxChannel(u8MuxChannel);
    if(stdResult != E_OK) {
        strSensorStatus[u8SensorId].u8LastReadStatus = cu8SENSOR_READ_ERROR;
        return stdResult;
    }
    
    /* Read multiplexer output */
    stdResult = stdReadMuxOutput(u8MuxNumber, &u8ReadState);
    if(stdResult != E_OK) {
        strSensorStatus[u8SensorId].u8LastReadStatus = cu8SENSOR_READ_ERROR;
        return stdResult;
    }
    
    /* Update sensor status */
    strSensorStatus[u8SensorId].u8SensorState = u8ReadState;
    strSensorStatus[u8SensorId].u8LastReadStatus = cu8SENSOR_READ_OK;
    strSensorStatus[u8SensorId].u16LastReadTime = u16GetTimeMs();
    
    *pu8SensorState = u8ReadState;
    return stdResult;
}

static Std_ReturnType_t stdSetMuxChannel(uint8_t u8Channel) {
    /* Set select pins A, B, C according to channel number (0-7) */
    DIO_WritePin(cstrMux_Config.enuSelectPinA_Port, cstrMux_Config.u8SelectPinA_Pin, (u8Channel & 0x01) ? STATE_HIGH : STATE_LOW);
    DIO_WritePin(cstrMux_Config.enuSelectPinB_Port, cstrMux_Config.u8SelectPinB_Pin, (u8Channel & 0x02) ? STATE_HIGH : STATE_LOW);
    DIO_WritePin(cstrMux_Config.enuSelectPinC_Port, cstrMux_Config.u8SelectPinC_Pin, (u8Channel & 0x04) ? STATE_HIGH : STATE_LOW);
    
    return E_OK;
}

static Std_ReturnType_t stdReadMuxOutput(uint8_t u8MuxNumber, uint8_t* pu8State) {
    if(u8MuxNumber == 1) {
        *pu8State = DIO_ReadPin(cstrMux_Config.enuOutput1_Port, cstrMux_Config.u8Output1_Pin);
    } else {
        *pu8State = DIO_ReadPin(cstrMux_Config.enuOutput2_Port, cstrMux_Config.u8Output2_Pin);
    }
    
    return E_OK;
}

Std_ReturnType_t SensorManager_stdReadAllSensors(void) {
    uint8_t u8I;
    uint8_t u8State;
    Std_ReturnType_t stdResult = E_OK;
    
    for(u8I = 0; u8I < cu8SENSOR_COUNT; u8I++) {
        if(SensorManager_stdReadSensor(u8I, &u8State) != E_OK) {
            stdResult = E_NOT_OK;
        }
    }
    
    return stdResult;
}

static uint16_t u16GetTimeMs(void) {
    Timer_GetValue(1);
}