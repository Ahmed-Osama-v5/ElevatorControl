

#define ModeManager_c

#include "Std_Types.h"
#include "Timer.h"
#include "dio.h"
#include "ModeManager.h"
#include "ModeManager_lcfg.h"

/* Private Variables */
static ModeStatus_t strModeStatus;
static boolean bIsTransitionInProgress = False;
static uint16_t u16TransitionStartTime = 0;

/* Private Function Declarations */
static Std_ReturnType_t stdValidateTransition(ModeState_t enuCurrentMode, ModeState_t enuNewMode);
static void vidExecuteTransition(ModeState_t enuNewMode);
static uint16_t u16GetTimeMs(void);
static ModeState_t enuReadModeSwitches(void);
static void vidNotifyModeChange(ModeState_t enuNewMode);

const ModeSwitchConfig_t     strMode_SwitchConfig = {
    .u8NormalModePin = 0,
    .u8ProgramModePin = 0,
    .u8MaintenanceModePin = 0,
    .enuGPIO = GPIOA
};
const OperationPermission_t  strMode_OperationPermissions[cu8OPERATION_COUNT] = {
    {True,  False, False},  /* Operation 0: Normal Mode */
    {False, True,  False},  /* Operation 1: Program Mode */
    {False, False, True},   /* Operation 2: Maintenance Mode */
    {True,  True,  True}    /* Operation 3: All Modes */
};

void ModeManager_Init(void)
{
    /* Initialize GPIO for mode switches */
    Dio_Cfg_t dio_config = {
        .enuGPIO = strMode_SwitchConfig.enuGPIO,
        .u8Pin = (1 << strMode_SwitchConfig.u8NormalModePin) |
                (1 << strMode_SwitchConfig.u8ProgramModePin) |
                (1 << strMode_SwitchConfig.u8MaintenanceModePin),
        .enuPinDir = DIR_INPUT
    };
    
    DIO_Init(&dio_config);
    
    /* Initialize mode status */
    strModeStatus.enuCurrentMode = MODE_NORMAL;
    strModeStatus.u16ModeEntryTime = u16GetTimeMs();
    strModeStatus.u8ErrorCode = cu8MODE_ERROR_NONE;
    strModeStatus.bIsInitialized = True;
    
    /* Initial mode reading */
    ModeState_t enuInitialMode = enuReadModeSwitches();
    if(enuInitialMode != MODE_ERROR) {
        strModeStatus.enuCurrentMode = enuInitialMode;
    }
}

ModeChangeResult_t ModeManager_enuRequestMode(ModeState_t enuNewMode)
{
    ModeChangeResult_t enuResult = MODE_CHANGE_OK;
    
    if(!strModeStatus.bIsInitialized) {
        enuResult = MODE_CHANGE_ERROR;
    }
    
    /* Validate mode transition */
    if(stdValidateTransition(strModeStatus.enuCurrentMode, enuNewMode) != E_OK) {
        enuResult = MODE_CHANGE_INVALID;
    }
    
    /* Check if transition is allowed */
    if(bIsTransitionInProgress) {
        enuResult = MODE_CHANGE_BLOCKED;
    }
    
    /* Start transition */
    bIsTransitionInProgress = True;
    u16TransitionStartTime = u16GetTimeMs();
    vidExecuteTransition(enuNewMode);
    
    return enuResult;
}

ModeState_t ModeManager_enuGetCurrentMode(void)
{
    return strModeStatus.enuCurrentMode;
}

Std_ReturnType_t ModeManager_Process(void)
{
    Std_ReturnType_t stdResult = E_OK;
    ModeState_t enuSwitchMode;
    uint16_t u16CurrentTime;
    
    if(!strModeStatus.bIsInitialized) {
        stdResult = E_NOT_OK;
    }
    
    /* Check mode switches */
    enuSwitchMode = enuReadModeSwitches();
    if(enuSwitchMode != MODE_ERROR && enuSwitchMode != strModeStatus.enuCurrentMode) {
        ModeManager_enuRequestMode(enuSwitchMode);
    }
    
    /* Handle transition timeout */
    if(bIsTransitionInProgress) {
        u16CurrentTime = u16GetTimeMs();
        if((u16CurrentTime - u16TransitionStartTime) >= cu16MODE_TRANSITION_TIMEOUT) {
            strModeStatus.u8ErrorCode = cu8MODE_ERROR_TIMEOUT;
            bIsTransitionInProgress = False;
            vidNotifyModeChange(MODE_ERROR);
        }
    }
    
    return stdResult;
}

boolean ModeManager_bIsAllowedOperation(uint8_t u8OperationId)
{
    boolean bResult = False;
    if(u8OperationId >= cu8OPERATION_COUNT) {
        bResult = False;
    }
    
    switch(strModeStatus.enuCurrentMode) {
        case MODE_NORMAL:
            bResult = strMode_OperationPermissions[u8OperationId].bNormalMode;
            break;
            
        case MODE_PROGRAM:
            bResult = strMode_OperationPermissions[u8OperationId].bProgramMode;
            break;
            
        case MODE_MAINTENANCE:
            bResult = strMode_OperationPermissions[u8OperationId].bMaintenanceMode;
            break;
            
        default:
            bResult = False;
            break;
    }

    return bResult;
}

Std_ReturnType_t ModeManager_GetStatus(ModeStatus_t* pstrStatus)
{
    Std_ReturnType_t stdResult = E_NOT_OK;
    if(pstrStatus == NULL || !strModeStatus.bIsInitialized) {
        stdResult = E_NOT_OK;
    }
    else
    {
        *pstrStatus = strModeStatus;
        stdResult = E_OK;
    }

    return stdResult;
}

Std_ReturnType_t ModeManager_HandleError(uint8_t u8ErrorCode)
{
    Std_ReturnType_t stdResult = E_NOT_OK;

    if(!strModeStatus.bIsInitialized) {
        stdResult = E_NOT_OK;
    }
    else
    {
        strModeStatus.u8ErrorCode = u8ErrorCode;
        if(u8ErrorCode != cu8MODE_ERROR_NONE) {
            strModeStatus.enuCurrentMode = MODE_ERROR;
            vidNotifyModeChange(MODE_ERROR);
        }
        
        stdResult = E_OK;
    }
    return stdResult;
}

/* Private Function Implementations */
static Std_ReturnType_t stdValidateTransition(ModeState_t enuCurrentMode, ModeState_t enuNewMode) {
    /* Add specific transition validation logic here */
    if(enuCurrentMode == MODE_ERROR && enuNewMode != MODE_NORMAL) {
        return E_NOT_OK;
    }
    return E_OK;
}

static void vidExecuteTransition(ModeState_t enuNewMode) {
    /* Perform any necessary cleanup for current mode */
    switch(strModeStatus.enuCurrentMode) {
        case MODE_NORMAL:
            /* Normal mode cleanup */
            break;
            
        case MODE_PROGRAM:
            /* Program mode cleanup */
            break;
            
        case MODE_MAINTENANCE:
            /* Maintenance mode cleanup */
            break;
            
        default:
            break;
    }
    
    /* Update mode */
    strModeStatus.enuCurrentMode = enuNewMode;
    strModeStatus.u16ModeEntryTime = u16GetTimeMs();
    
    /* Notify other modules about mode change */
    vidNotifyModeChange(enuNewMode);
    
    bIsTransitionInProgress = False;
}

static ModeState_t enuReadModeSwitches(void) {
    PinState_t enuNormalState = DIO_ReadPin(strMode_SwitchConfig.enuGPIO, strMode_SwitchConfig.u8NormalModePin);
    PinState_t enuProgramState = DIO_ReadPin(strMode_SwitchConfig.enuGPIO, strMode_SwitchConfig.u8ProgramModePin);
    PinState_t enuMaintState = DIO_ReadPin(strMode_SwitchConfig.enuGPIO, strMode_SwitchConfig.u8MaintenanceModePin);
    
    /* Only one switch should be active at a time */
    if(enuNormalState && !enuProgramState && !enuMaintState) {
        return MODE_NORMAL;
    }
    else if(!enuNormalState && enuProgramState && !enuMaintState) {
        return MODE_PROGRAM;
    }
    else if(!enuNormalState && !enuProgramState && enuMaintState) {
        return MODE_MAINTENANCE;
    }
    
    return MODE_ERROR;
}

static void vidNotifyModeChange(ModeState_t enuNewMode) {
    /* Implement notifications to other modules */
    /* This could be through callback functions or a message queue */
}

static uint16_t u16GetTimeMs(void) {
    return Timer_GetValue(1);
}
