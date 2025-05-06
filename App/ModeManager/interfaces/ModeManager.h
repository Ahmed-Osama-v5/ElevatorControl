#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include "Std_Types.h"
#include "ModeManager_gcfg.h"


/* Function Declarations */
void ModeManager_Init(void);
ModeChangeResult_t ModeManager_enuRequestMode(ModeState_t enuNewMode);
ModeState_t ModeManager_enuGetCurrentMode(void);
Std_ReturnType_t ModeManager_Process(void);
boolean ModeManager_bIsAllowedOperation(uint8_t u8OperationId);
Std_ReturnType_t ModeManager_GetStatus(ModeStatus_t* pstrStatus);
Std_ReturnType_t ModeManager_HandleError(uint8_t u8ErrorCode);


#endif // MODE_MANAGER_H
