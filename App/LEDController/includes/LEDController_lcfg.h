#ifndef LED_CONTROLLER_LCFG_H
#define LED_CONTROLLER_LCFG_H

#if ! ((defined LEDController_c) || (defined LEDController_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* Private defines */
#define cu8SHIFT_REG_DELAY_US ((uint8_t) 1U)

/* Module private types */
typedef struct {
    LEDState_t enuState;
    LEDPattern_t enuPattern;
    uint16_t u16LastToggleTime;
    boolean bCurrentOutput;
    boolean bIsInitialized;
} LEDStatus_t;


#endif // LED_CONTROLLER_LCFG_H
