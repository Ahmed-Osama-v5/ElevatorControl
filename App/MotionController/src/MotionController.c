
#include "RelayManager_gcfg.h"
#define MotionController_c

#include "Std_Types.h"
#include "MotionController.h"
#include "MotionController_lcfg.h"
#include "RelayManager.h"
#include "Timer.h"
#include "SensorManager.h"


/* Private Types */
typedef struct {
    MotionState_t enuCurrentState;
    uint8_t u8TargetFloor;
    uint8_t u8CurrentFloor;
    uint16_t u16LastMotionTime;
    boolean bInitialized;
} MotionController_StatusType;

/* Private Variables */
static MotionController_StatusType strMotionStatus;

/* Private Function Declarations */
static void vidControlRelays(boolean bUp, boolean bDown, boolean bLS, boolean bHS);
static boolean bIsTargetFloorReached(void);
static boolean bIsFloorMarkReached(void);
static void vidUpdateCurrentFloor(void);
static uint16_t u16GetTimeMs(void);

/* Function Implementations */
Std_ReturnType_t MotionController_stdInit(void)
{
    Std_ReturnType_t stdRetVal = E_OK;
    
    /* Initialize Relays */
    RelayManager_vidInit();
    
    /* Initialize status */
    strMotionStatus.enuCurrentState = MOTION_STATE_IDLE;
    strMotionStatus.u8TargetFloor = 1U;
    strMotionStatus.u8CurrentFloor = 1U;
    strMotionStatus.u16LastMotionTime = 0U;
    strMotionStatus.bInitialized = TRUE;
    
    return stdRetVal;
}

Std_ReturnType_t MotionController_stdSetTargetFloor(uint8_t u8Floor)
{
    if ((u8Floor == 0) || (u8Floor > cu8MAX_FLOOR) || !strMotionStatus.bInitialized)
    {
        return E_NOT_OK;
    }
    
    strMotionStatus.u8TargetFloor = u8Floor;
    return E_OK;
}

Std_ReturnType_t MotionController_stdExecuteCommand(MotionCommand_t enuCommand)
{
    if (!strMotionStatus.bInitialized)
    {
        return E_NOT_OK;
    }
    
    switch (enuCommand)
    {
        case MOTION_CMD_STOP:
            vidControlRelays(FALSE, FALSE, FALSE, FALSE);
            strMotionStatus.enuCurrentState = MOTION_STATE_IDLE;
            break;
            
        case MOTION_CMD_MOVE_UP:
            if (strMotionStatus.u8CurrentFloor < strMotionStatus.u8TargetFloor)
            {
                /* Start moving up at high speed */
                vidControlRelays(TRUE, FALSE, FALSE, TRUE);
                strMotionStatus.enuCurrentState = MOTION_STATE_MOVING_UP_HS;
                strMotionStatus.u16LastMotionTime = u16GetTimeMs();
            }
            break;
            
        case MOTION_CMD_MOVE_DOWN:
            if (strMotionStatus.u8CurrentFloor > strMotionStatus.u8TargetFloor)
            {
                /* Start moving down at high speed */
                vidControlRelays(FALSE, TRUE, FALSE, TRUE);
                strMotionStatus.enuCurrentState = MOTION_STATE_MOVING_DOWN_HS;
                strMotionStatus.u16LastMotionTime = u16GetTimeMs();
            }
            break;
            
        default:
            return E_NOT_OK;
    }
    
    return E_OK;
}

Std_ReturnType_t MotionController_stdProcess(void)
{
    uint8_t u8SensorState;
    
    if (!strMotionStatus.bInitialized)
    {
        return E_NOT_OK;
    }
    
    /* Update current floor based on sensors */
    vidUpdateCurrentFloor();
    
    /* Check motion timeout */
    if ((strMotionStatus.enuCurrentState != MOTION_STATE_IDLE) &&
        ((u16GetTimeMs() - strMotionStatus.u16LastMotionTime) > cu16MOTION_TIMEOUT_MS))
    {
        MotionController_stdExecuteCommand(MOTION_CMD_STOP);
        strMotionStatus.enuCurrentState = MOTION_STATE_ERROR;
        return E_NOT_OK;
    }
    
    /* Process based on current state */
    switch (strMotionStatus.enuCurrentState)
    {
        case MOTION_STATE_MOVING_UP_HS:
            if (bIsTargetFloorReached())
            {
                /* Switch to low speed */
                vidControlRelays(TRUE, FALSE, TRUE, FALSE);
                strMotionStatus.enuCurrentState = MOTION_STATE_MOVING_UP_LS;
            }
            break;
            
        case MOTION_STATE_MOVING_UP_LS:
            if (bIsFloorMarkReached())
            {
                MotionController_stdExecuteCommand(MOTION_CMD_STOP);
            }
            break;
            
        case MOTION_STATE_MOVING_DOWN_HS:
            if (bIsTargetFloorReached())
            {
                /* Switch to low speed */
                vidControlRelays(FALSE, TRUE, TRUE, FALSE);
                strMotionStatus.enuCurrentState = MOTION_STATE_MOVING_DOWN_LS;
            }
            break;
            
        case MOTION_STATE_MOVING_DOWN_LS:
            if (bIsFloorMarkReached())
            {
                MotionController_stdExecuteCommand(MOTION_CMD_STOP);
            }
            break;
            
        default:
            break;
    }
    
    return E_OK;
}

static void vidControlRelays(boolean bUp, boolean bDown, boolean bLS, boolean bHS)
{
    if(bUp)
    {
        RelayManager_vidActivateRelay(RELAY_UP);
    }
    else
    {
        RelayManager_vidDeActivateRelay(RELAY_UP);
    }

    if(bDown)
    {
        RelayManager_vidActivateRelay(RELAY_DN);
    }
    else
    {
        RelayManager_vidDeActivateRelay(RELAY_DN);
    }

    if(bLS)
    {
        RelayManager_vidActivateRelay(RELAY_LS);
    }
    else
    {
        RelayManager_vidDeActivateRelay(RELAY_LS);
    }

    if(bHS)
    {
        RelayManager_vidActivateRelay(RELAY_HS);
    }
    else
    {
        RelayManager_vidDeActivateRelay(RELAY_HS);
    }
}

static boolean bIsTargetFloorReached(void)
{
    PinState_t enuReadState;
    
    /* Read floor sensor for target floor */
    SensorManager_stdReadSensor(cu8SENSOR_SELECTOR, &enuReadState);
    return (enuReadState == cu8SENSOR_STATE_ACTIVE);
}

static boolean bIsFloorMarkReached(void)
{
    PinState_t enuReadState;

    /* Read floor mark sensor */
    SensorManager_stdReadSensor(cu8SENSOR_FLOOR_M, &enuReadState);
    return (enuReadState == cu8SENSOR_STATE_ACTIVE);
}

static void vidUpdateCurrentFloor(void)
{
    PinState_t enuReadState;

    /* Update current floor based on floor mark sensor */
    if (bIsFloorMarkReached())
    {
        if (strMotionStatus.enuCurrentState == MOTION_STATE_MOVING_UP_LS ||
            strMotionStatus.enuCurrentState == MOTION_STATE_MOVING_UP_HS)
        {
            strMotionStatus.u8CurrentFloor++;
        }
        else if (strMotionStatus.enuCurrentState == MOTION_STATE_MOVING_DOWN_LS ||
                 strMotionStatus.enuCurrentState == MOTION_STATE_MOVING_DOWN_HS)
        {
            strMotionStatus.u8CurrentFloor--;
        }
    }
}

static uint16_t u16GetTimeMs(void)
{
    return Timer_GetValue(1);
}