
#define Diagnostics_c


#include "Std_Types.h"
#include "Diagnostics.h"
#include "Diagnostics_lcfg.h"
#include "Timer.h"
#include "SensorManager.h"
#include "char_lcd.h"


/* Private Variables */
static DiagEventLog_t astrEventLog[cu8MAX_EVENT_LOG_SIZE];
static uint8_t u8EventLogIndex = 0;
static DiagComponentStatus_t astrComponentStatus[6];  /* One for each component */
static DiagMaintenanceData_t strMaintenanceData;
static boolean bInitialized = FALSE;
static uint16_t u16LastHealthCheck = 0;

/* Private Function Declarations */
static void vidUpdateSystemHealth(void);
static void vidCheckMaintenanceStatus(void);
static uint16_t u16GetTimeMs(void);
static void vidDisplayError(DiagComponent_t enuComponent, DiagError_t enuError);
static boolean bIsErrorCritical(DiagError_t enuError);
static void vidLogEvent(DiagEvent_t enuEvent, uint16_t u16Data);

/* Function Implementations */
Std_ReturnType_t Diagnostics_Init(void)
{
    uint8_t u8Index;
    
    if(bInitialized)
    {
        return E_NOT_OK;
    }
    
    /* Initialize component status array */
    for(u8Index = 0; u8Index < 6; u8Index++)
    {
        astrComponentStatus[u8Index].enuCurrentError = DIAG_ERR_NONE;
        astrComponentStatus[u8Index].u16ErrorTime = 0;
        astrComponentStatus[u8Index].u8ErrorCount = 0;
        astrComponentStatus[u8Index].u8ComponentStatus = cu8COMP_STATUS_NORMAL;
    }
    
    /* Initialize maintenance data */
    strMaintenanceData.u16DoorCycles = 0;
    strMaintenanceData.u16TravelCycles = 0;
    strMaintenanceData.u16OperatingTime = 0;
    strMaintenanceData.bMaintenanceNeeded = FALSE;
    
    /* Clear event log */
    for(u8Index = 0; u8Index < cu8MAX_EVENT_LOG_SIZE; u8Index++)
    {
        astrEventLog[u8Index].enuEvent = DIAG_EVENT_SYSTEM_RESET;
        astrEventLog[u8Index].u16Data = 0;
        astrEventLog[u8Index].u16Timestamp = 0;
    }
    
    u8EventLogIndex = 0;
    u16LastHealthCheck = u16GetTimeMs();
    bInitialized = TRUE;
    
    /* Log system initialization */
    vidLogEvent(DIAG_EVENT_SYSTEM_RESET, 0);
    
    return E_OK;
}

Std_ReturnType_t Diagnostics_ReportError(DiagComponent_t enuComponent, DiagError_t enuError)
{
    if(!bInitialized || enuComponent >= DIAG_COMP_SENSOR_MANAGER)
    {
        return E_NOT_OK;
    }
    
    /* Update component error status */
    astrComponentStatus[enuComponent].enuCurrentError = enuError;
    astrComponentStatus[enuComponent].u16ErrorTime = u16GetTimeMs();
    astrComponentStatus[enuComponent].u8ErrorCount++;
    
    /* Update component status based on error severity */
    if(bIsErrorCritical(enuError))
    {
        astrComponentStatus[enuComponent].u8ComponentStatus = cu8COMP_STATUS_CRITICAL;
    }
    else
    {
        astrComponentStatus[enuComponent].u8ComponentStatus = cu8COMP_STATUS_ERROR;
    }
    
    /* Log error event */
    vidLogEvent(DIAG_EVENT_ERROR_OCCURRED, ((uint16_t)enuComponent << 8) | enuError);
    
    /* Display error on LCD */
    vidDisplayError(enuComponent, enuError);
    
    return E_OK;
}

Std_ReturnType_t Diagnostics_Process(void)
{
    uint32_t u16CurrentTime;
    PinState_t enuSensorState;
    
    if(!bInitialized)
    {
        return E_NOT_OK;
    }
    
    u16CurrentTime = u16GetTimeMs();
    
    /* Periodic health check */
    if((u16CurrentTime - u16LastHealthCheck) >= cu16HEALTH_CHECK_INTERVAL_MS)
    {
        vidUpdateSystemHealth();
        vidCheckMaintenanceStatus();
        u16LastHealthCheck = u16CurrentTime;
    }
    
    /* Check critical sensors */
    SensorManager_stdReadSensor(cu8SENSOR_PHASE_1, &enuSensorState);
    if(enuSensorState == cenuSENSOR_STATE_ACTIVE)
    {
        Diagnostics_ReportError(DIAG_COMP_MOTION_CTRL, DIAG_ERR_PHASE_FAULT);
    }
    
    SensorManager_stdReadSensor(cu8SENSOR_STOP_SEN, &enuSensorState);
    if(enuSensorState == cenuSENSOR_STATE_ACTIVE)
    {
        Diagnostics_ReportError(DIAG_COMP_MOTION_CTRL, DIAG_ERR_STOP_ACTIVATED);
    }
    
    SensorManager_stdReadSensor(cu8SENSOR_OVER_LOAD, &enuSensorState);
    if(enuSensorState == cenuSENSOR_STATE_ACTIVE)
    {
        Diagnostics_ReportError(DIAG_COMP_MOTION_CTRL, DIAG_ERR_OVERLOAD);
    }
    
    return E_OK;
}

static void vidUpdateSystemHealth(void)
{
    uint8_t u8Component;
    uint8_t u8WorstStatus = cu8COMP_STATUS_NORMAL;
    
    /* Check all components and find worst status */
    for(u8Component = 0; u8Component < 6; u8Component++)
    {
        if(astrComponentStatus[u8Component].u8ComponentStatus > u8WorstStatus)
        {
            u8WorstStatus = astrComponentStatus[u8Component].u8ComponentStatus;
        }
        
        /* Clear transient errors after debounce time */
        if((u16GetTimeMs() - astrComponentStatus[u8Component].u16ErrorTime) > cu16ERROR_DEBOUNCE_MS)
        {
            if(astrComponentStatus[u8Component].enuCurrentError != DIAG_ERR_NONE)
            {
                astrComponentStatus[u8Component].enuCurrentError = DIAG_ERR_NONE;
                astrComponentStatus[u8Component].u8ComponentStatus = cu8COMP_STATUS_NORMAL;
            }
        }
    }
}

static void vidCheckMaintenanceStatus(void)
{
    /* Check maintenance thresholds */
    if(strMaintenanceData.u16DoorCycles >= cu16DOOR_CYCLES_MAINTENANCE ||
       strMaintenanceData.u16TravelCycles >= cu16TRAVEL_CYCLES_MAINTENANCE)
    {
        if(!strMaintenanceData.bMaintenanceNeeded)
        {
            strMaintenanceData.bMaintenanceNeeded = TRUE;
            vidLogEvent(DIAG_EVENT_MAINTENANCE_DUE, 0);
        }
    }
}

static void vidDisplayError(DiagComponent_t enuComponent, DiagError_t enuError)
{
    /* Clear LCD */
    LCD_send_command(Lcd_clear);
    
    /* Display error information */
    LCD_send_string("Error: ");
    
    switch(enuComponent)
    {
        case DIAG_COMP_MOTION_CTRL:
            LCD_send_string("Motion");
            break;
        case DIAG_COMP_DOOR_CTRL:
            LCD_send_string("Door");
            break;
        case DIAG_COMP_CALL_HANDLER:
            LCD_send_string("Call");
            break;
        case DIAG_COMP_MODE_MANAGER:
            LCD_send_string("Mode");
            break;
        case DIAG_COMP_LED_CTRL:
            LCD_send_string("LED");
            break;
        case DIAG_COMP_SENSOR_MANAGER:
            LCD_send_string("Sensor");
            break;
    }
    
    /* Move to second line */
    LCD_goto_xy(0, LINE_1);
    
    /* Display error code */
    switch(enuError)
    {
        case DIAG_ERR_DOOR_TIMEOUT:
            LCD_send_string("Door Timeout");
            break;
        case DIAG_ERR_MOTION_TIMEOUT:
            LCD_send_string("Motion Timeout");
            break;
        case DIAG_ERR_OVERLOAD:
            LCD_send_string("Overload");
            break;
        case DIAG_ERR_PHASE_FAULT:
            LCD_send_string("Phase Fault");
            break;
        case DIAG_ERR_LOCK_FAULT:
            LCD_send_string("Lock Fault");
            break;
        case DIAG_ERR_STOP_ACTIVATED:
            LCD_send_string("Stop Active");
            break;
        default:
            LCD_send_string("Unknown Error");
            break;
    }
}

static boolean bIsErrorCritical(DiagError_t enuError)
{
    switch(enuError)
    {
        case DIAG_ERR_PHASE_FAULT:
        case DIAG_ERR_STOP_ACTIVATED:
        case DIAG_ERR_LOCK_FAULT:
        case DIAG_ERR_OVERLOAD:
            return TRUE;
        default:
            return FALSE;
    }
}

static void vidLogEvent(DiagEvent_t enuEvent, uint16_t u16Data)
{
    /* Add event to circular buffer */
    astrEventLog[u8EventLogIndex].enuEvent = enuEvent;
    astrEventLog[u8EventLogIndex].u16Data = u16Data;
    astrEventLog[u8EventLogIndex].u16Timestamp = u16GetTimeMs();
    
    /* Update index */
    u8EventLogIndex = (u8EventLogIndex + 1) % cu8MAX_EVENT_LOG_SIZE;
}

static uint16_t u16GetTimeMs(void)
{
    return Timer_GetValue(1);
}