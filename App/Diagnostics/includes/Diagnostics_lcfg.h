#ifndef DIAGNOSTICS_LCFG_H
#define DIAGNOSTICS_LCFG_H

#if ! ((defined Diagnostics_c) || (defined Diagnostics_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif


/* Private Types */
typedef struct {
    DiagEvent_t enuEvent;
    uint16_t u16Data;
    uint16_t u16Timestamp;
} DiagEventLog_t;

typedef struct {
    DiagError_t enuCurrentError;
    uint16_t u16ErrorTime;
    uint8_t u8ErrorCount;
    uint8_t u8ComponentStatus;
} DiagComponentStatus_t;

typedef struct {
    uint16_t u16DoorCycles;
    uint16_t u16TravelCycles;
    uint16_t u16OperatingTime;
    boolean bMaintenanceNeeded;
} DiagMaintenanceData_t;


#endif // DIAGNOSTICS_LCFG_H
