#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "Diagnostics_gcfg.h"


#include "Std_Types.h"

/* Function Declarations */
Std_ReturnType_t Diagnostics_Init(void);
Std_ReturnType_t Diagnostics_DeInit(void);
Std_ReturnType_t Diagnostics_ReportError(DiagComponent_t enuComponent, DiagError_t enuError);
Std_ReturnType_t Diagnostics_ClearError(DiagComponent_t enuComponent, DiagError_t enuError);
Std_ReturnType_t Diagnostics_LogEvent(DiagEvent_t enuEvent, uint16_t u16Data);
Std_ReturnType_t Diagnostics_GetErrorStatus(DiagComponent_t enuComponent, DiagError_t* penuError);
Std_ReturnType_t Diagnostics_Process(void);
Std_ReturnType_t Diagnostics_GetSystemHealth(uint8_t* pu8HealthStatus);
Std_ReturnType_t Diagnostics_GetMaintenanceStatus(uint8_t* pu8MaintenanceNeeded);
Std_ReturnType_t Diagnostics_GetErrorCount(uint16_t* pu16Count);
Std_ReturnType_t Diagnostics_GetEventLog(uint8_t u8Index, DiagEvent_t* penuEvent, uint16_t* pu16Data);


#endif // DIAGNOSTICS_H
