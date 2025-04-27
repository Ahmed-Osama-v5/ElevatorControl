#ifndef CALL_HANDLER_H
#define CALL_HANDLER_H

#include "CallHandler_gcfg.h"
#include "LEDController_gcfg.h"
//#include "ElevatorController_gcfg.h"


void CallHandler_vidInit(void);
void CallHandler_vidProcess(Elevator_t* pstrElevator);
boolean CallHandler_bRegister_call(uint8_t u8Floor, CallType_t enuCallType, MoveDirection_t enuDirection);
void CallHandler_vidClear_call(uint8_t u8Floor);
boolean CallHandler_bHas_calls_above(uint8_t u8CurrentFloor);
boolean CallHandler_bHas_calls_below(uint8_t u8CurrentFloor);
uint8_t CallHandler_u8Get_next_floor(uint8_t u8CurrentFloor, MoveDirection_t enuDirection);
void CallHandler_vidProcess_leds(void);


#endif // CALL_HANDLER_H
