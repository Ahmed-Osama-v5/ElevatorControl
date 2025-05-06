#ifndef MODE_MANAGER_LCFG_H
#define MODE_MANAGER_LCFG_H

#include "ModeManager_gcfg.h"
#include "dio_types.h"

#if ! ((defined ModeManager_c) || (defined ModeManager_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* Mode Switch Configuration */
typedef struct {
    uint8_t u8NormalModePin;
    uint8_t u8ProgramModePin;
    uint8_t u8MaintenanceModePin;
    GPIO_t enuGPIO;
} ModeSwitchConfig_t;

/* Operation Permissions Matrix */
typedef struct {
    boolean bNormalMode;
    boolean bProgramMode;
    boolean bMaintenanceMode;
} OperationPermission_t;

extern const ModeSwitchConfig_t     strMode_SwitchConfig;
extern const OperationPermission_t  strMode_OperationPermissions[cu8OPERATION_COUNT];

#endif // MODE_MANAGER_LCFG_H
