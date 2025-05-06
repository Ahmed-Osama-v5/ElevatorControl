#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include "Std_Types.h"
#include "MotionController_gcfg.h"

/* Function Declarations */
Std_ReturnType_t MotionController_stdInit(void);
Std_ReturnType_t MotionController_stdDeInit(void);
Std_ReturnType_t MotionController_stdSetTargetFloor(uint8_t u8Floor);
Std_ReturnType_t MotionController_stdExecuteCommand(MotionCommand_t enuCommand);
Std_ReturnType_t MotionController_stdGetCurrentState(MotionState_t* penuState);
Std_ReturnType_t MotionController_stdProcess(void);
uint8_t MotionController_u8GetCurrentFloor(void);


#endif // MOTION_CONTROLLER_H
