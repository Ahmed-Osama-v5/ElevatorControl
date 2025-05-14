#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "Std_Types.h"
#include "dio.h"
#include "Timer.h"
#include "LEDController_gcfg.h"


/* Function Declarations */
void LEDController_vidInit(void);
void LEDController_vidSetState(uint8_t u8LedId, LEDState_t enuState);
void LEDController_vidSetPattern(uint8_t u8LedId, LEDPattern_t enuPattern);
void LEDController_vidProcess(void);


#endif // LED_CONTROLLER_H
