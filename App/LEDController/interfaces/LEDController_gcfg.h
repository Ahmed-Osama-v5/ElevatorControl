#ifndef LED_CONTROLLER_GCFG_H
#define LED_CONTROLLER_GCFG_H

typedef struct
{
    boolean bIsActive;
    CallType_t enuCallType;
    uint16_t u16BlinkTimer;
    boolean bLEDState;
}FloorLED_t;


#endif // LED_CONTROLLER_GCFG_H
