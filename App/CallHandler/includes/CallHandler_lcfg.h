#ifndef CALL_HANDLER_LCFG_H
#define CALL_HANDLER_LCFG_H

#if ! ((defined CallHandler_c) || (defined CallHandler_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

#define cu16LED_BLINK_PERIOD_MS     ((uint16_t) 500U)       // Base blink period
#define cu8LED_INT_DUTY_CYCLE       ((uint8_t) 50U)         // Internal call LED duty cycle (%)
#define cu8LED_EXT_DUTY_CYCLE       ((uint8_t) 75U)         // External call LED duty cycle (%)


#endif // CALL_HANDLER_LCFG_H
