
#include "SensorManager_gcfg.h"
#include "SystemConfig.h"
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
    .enuSelectPin1_Port = GPIOB,
    .u8SelectPin1_Pin = 6,
    .enuSelectPin2_Port = GPIOB,
    .u8SelectPin2_Pin = 5,
    .enuSelectPin3_Port = GPIOB,
    .u8SelectPin3_Pin = 4,
    .enuOutput0_Port = GPIOD,
    .u8Output0_Pin = 7,
    .enuOutput1_Port = GPIOB,
    .u8Output1_Pin = 7
};

/* Private Function Declarations */
static Std_ReturnType_t stdSetMuxChannel(uint8_t u8Channel);
static Std_ReturnType_t stdReadMuxOutput(uint8_t u8MuxNumber, PinState_t* penuState);

/* Function Implementations */
Std_ReturnType_t SensorManager_stdInit(void) {
    Std_ReturnType_t stdResult = E_OK;
    uint8_t u8I;
    
    if(bModuleInitialized == True) {
        return E_NOT_OK;
    }

    /* Initialize GPIO for multiplexer select pins */
    Dio_Cfg_t dio_config = {
        .enuGPIO = cstrMux_Config.enuSelectPin1_Port,
        .u8Pin = cstrMux_Config.u8SelectPin1_Pin,
        .enuPinDir = DIR_OUTPUT
    };
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuSelectPin2_Port;
    dio_config.u8Pin = cstrMux_Config.u8SelectPin2_Pin;
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuSelectPin3_Port;
    dio_config.u8Pin = cstrMux_Config.u8SelectPin3_Pin;
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cstrMux_Config.enuOutput0_Port;
    dio_config.u8Pin = cstrMux_Config.u8Output0_Pin;
    DIO_Init(&dio_config);
    
    /* Initialize GPIO for multiplexer outputs */
    dio_config.enuGPIO = cstrMux_Config.enuOutput1_Port;
    dio_config.u8Pin = cstrMux_Config.u8Output1_Pin;
    dio_config.enuPinDir = DIR_INPUT;
    DIO_Init(&dio_config);
    
    /* Initialize sensor status array */
    for(u8I = 0; u8I < cu8SENSOR_COUNT; u8I++) {
        strSensorStatus[u8I].enuSensorState = cu8SENSOR_STATE_INACTIVE;
        strSensorStatus[u8I].u8ActiveCounter = 0;
    }
    
    bModuleInitialized = True;
    return stdResult;
}

Std_ReturnType_t SensorManager_stdReadSensor(Sensor_t enuSensorId, PinState_t* penuSensorState) {
    Std_ReturnType_t stdResult = E_OK;
    uint8_t u8MuxChannel;
    uint8_t u8MuxNumber;
    PinState_t enuReadState;

    if(!bModuleInitialized || enuSensorId >= cu8SENSOR_COUNT || penuSensorState == NULL) {
        return E_NOT_OK;
    }
    
    /* Determine which multiplexer and channel to use */
    if(enuSensorId < 8) {
        u8MuxNumber = 1;  /* U12 - SEN_1_OUT */
        u8MuxChannel = enuSensorId;
    } else {
        u8MuxNumber = 2;  /* U11 - SEN_0_OUT */
        u8MuxChannel = enuSensorId - 8;
    }
    
    /* Set multiplexer channel */
    stdResult = stdSetMuxChannel(u8MuxChannel);
    
    /* Read multiplexer output */
    stdResult = stdReadMuxOutput(u8MuxNumber, &enuReadState);

    /* Debounce read status */
    if(enuReadState == cu8SENSOR_STATE_INACTIVE)
    {
        strSensorStatus[enuSensorId].u8ActiveCounter = 0;
        strSensorStatus[enuSensorId].enuSensorState = cu8SENSOR_STATE_INACTIVE;
        
        /* Update sensor state */
        *penuSensorState = cu8SENSOR_STATE_INACTIVE;
    }
    else
    {
        if(strSensorStatus[enuSensorId].u8ActiveCounter >= cu8SENSOR_DEBOUNCE_COUNTER)
        {
            strSensorStatus[enuSensorId].enuSensorState = cu8SENSOR_STATE_ACTIVE;
            strSensorStatus[enuSensorId].u8ActiveCounter = 0;
            
            /* Update sensor state */
            *penuSensorState = cu8SENSOR_STATE_ACTIVE;
        }
        else
        {
            strSensorStatus[enuSensorId].u8ActiveCounter++;
        }
    }

    return stdResult;
}

static Std_ReturnType_t stdSetMuxChannel(uint8_t u8Channel) {
    /* Set select pins A, B, C according to channel number (0-7) */
    DIO_WritePin(cstrMux_Config.enuSelectPin1_Port, cstrMux_Config.u8SelectPin1_Pin, (u8Channel & 0x01) ? STATE_HIGH : STATE_LOW);
    DIO_WritePin(cstrMux_Config.enuSelectPin2_Port, cstrMux_Config.u8SelectPin2_Pin, (u8Channel & 0x02) ? STATE_HIGH : STATE_LOW);
    DIO_WritePin(cstrMux_Config.enuSelectPin3_Port, cstrMux_Config.u8SelectPin3_Pin, (u8Channel & 0x04) ? STATE_HIGH : STATE_LOW);
    
    return E_OK;
}

static Std_ReturnType_t stdReadMuxOutput(uint8_t u8MuxNumber, PinState_t* penuState) {
    if(u8MuxNumber == 1) {
        *penuState = DIO_ReadPin(cstrMux_Config.enuOutput1_Port, cstrMux_Config.u8Output1_Pin);
    } else {
        *penuState = DIO_ReadPin(cstrMux_Config.enuOutput0_Port, cstrMux_Config.u8Output0_Pin);
    }
    
    return E_OK;
}

Std_ReturnType_t SensorManager_stdReadAllSensors(void) {
    uint8_t u8I;
    PinState_t enuState;
    Std_ReturnType_t stdResult = E_OK;
    
    for(u8I = 0; u8I < cu8SENSOR_COUNT; u8I++) {
        if(SensorManager_stdReadSensor(u8I, &enuState) != E_OK) {
            stdResult = E_NOT_OK;
        }
    }
    
    return stdResult;
}

uint8_t SensorManager_u8GetSensorCounter(Sensor_t enuSensorId)
{
    return strSensorStatus[enuSensorId].u8ActiveCounter;
}

inline char* toString(Sensor_t enuSensorId)
{
    switch(enuSensorId)
    {
        case FLOOR_M: return "FLM";
        case SELECTOR: return "SEL";
        case STOP_SEN: return "STP";
        case SHK: return "SHK";
        case LOCK: return "LOC";
        case UP_LIMIT: return "UPL";
        case DN_LIMIT: return "DNL";
        case MNT_SEN: return "MNT";
        case MNT_UP: return "MUP";
        case MNT_DN: return "MDN";
        case OPEN_DOOR: return "OPN";
        case CLOSE_DOOR: return "CLS";
        case OVER_LOAD: return "OVL";
        case FULL_LOAD: return "FUL";
        case PHASE_1: return "PH1";
        case PHASE_2: return "PH2";
        case NO_SENSOR: return "NOS";
        default: return "UNK";
    }
}
