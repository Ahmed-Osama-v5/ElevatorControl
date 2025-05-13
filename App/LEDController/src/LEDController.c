
#define LEDController_c

#include "Std_Types.h"
#include "LEDController.h"
#include "LEDController_lcfg.h"

/* Module private variables */
static LEDStatus_t enuLedStatus[cu8LED_MAX_COUNT];
static uint8_t au8LatchData[2] = {0};  // Data for two 74HC573 latches
static boolean bModuleInitialized = False;

/* Private function declarations */
static boolean bLED_ValidateId(uint8_t u8LedId);
static void vidLED_UpdateHardware(uint8_t u8LedId, boolean bState);
static uint16_t u16LED_GetCurrentTime(void);
static void vidLED_LatchUpdate(uint8_t u8LatchIndex);

const LEDHWConfig_t LED_HWConfig[cu8LED_MAX_COUNT] = {
    {0, 0}, // LED 0: Latch 0, Bit 0
    {0, 1}, // LED 1: Latch 0, Bit 1
    {0, 2}, // ...
    {0, 3},
    {0, 4},
    {0, 5},
    {0, 6},
    {0, 7},
    {1, 0}, // LED 8: Latch 1, Bit 0
    {1, 1},
    {1, 2},
    {1, 3},
    {1, 4},
    {1, 5},
    {1, 6},
    {1, 7}
};

static void vidLED_LatchUpdate(uint8_t u8LatchIndex) {
    // Output the data to the data port
    DIO_WritePort(cenuGPIOLED_DATA_PORT, au8LatchData[u8LatchIndex]);
    // Toggle the corresponding latch pin
    if (u8LatchIndex == 0) {
        DIO_WritePin(cenuGPIOLED_LATCH0_PORT, cu8LED_LATCH0_PIN, STATE_HIGH);
        // Small delay if needed
        DIO_WritePin(cenuGPIOLED_LATCH0_PORT, cu8LED_LATCH0_PIN, STATE_LOW);
    } else {
        DIO_WritePin(cenuGPIOLED_LATCH1_PORT, cu8LED_LATCH1_PIN, STATE_HIGH);
        // Small delay if needed
        DIO_WritePin(cenuGPIOLED_LATCH1_PORT, cu8LED_LATCH1_PIN, STATE_LOW);
    }
}

static void vidLED_UpdateHardware(uint8_t u8LedId, boolean bState) {
    uint8_t u8LatchIndex = LED_HWConfig[u8LedId].u8LatchIndex;
    uint8_t u8BitPos = LED_HWConfig[u8LedId].u8BitPos;
    
    if(bState) {
        au8LatchData[u8LatchIndex] |= (1 << u8BitPos);
    } else {
        au8LatchData[u8LatchIndex] &= ~(1 << u8BitPos);
    }
    
    vidLED_LatchUpdate(u8LatchIndex);
}

static boolean bLED_ValidateId(uint8_t u8LedId)
{
    if(u8LedId >= cu8LED_MAX_COUNT) {
        return False;
    }
    return True;
}

static uint16_t u16LED_GetCurrentTime(void) {
    /* This should be implemented based on your system's timing service */
    return Timer_GetValue(TIMER_CH1);
}

void LEDController_vidInit(void)
{
    uint8_t u8I;

    /* Initialize latch control pins */
    Dio_Cfg_t dio_config = {
        .enuGPIO = cenuGPIOLED_LATCH0_PORT,
        .u8Pin = cu8LED_LATCH0_PIN,
        .enuPinDir = DIR_OUTPUT
    };
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cenuGPIOLED_LATCH1_PORT;
    dio_config.u8Pin = cu8LED_LATCH1_PIN;
    DIO_Init(&dio_config);

    /* Initialize data port */
    dio_config.enuGPIO = cenuGPIOLED_DATA_PORT;
    dio_config.u8Pin = 0xFF;  // All pins as output
    DIO_Init(&dio_config);
    
    /* Initialize LED states */
    for(u8I = 0; u8I < cu8LED_MAX_COUNT; u8I++) {
        enuLedStatus[u8I].enuState = LED_STATE_OFF;
        enuLedStatus[u8I].enuPattern = LED_PATTERN_NONE;
        enuLedStatus[u8I].u16LastToggleTime = 0;
        enuLedStatus[u8I].bCurrentOutput = False;
        enuLedStatus[u8I].bIsInitialized = True;
        
        vidLED_UpdateHardware(u8I, False);
    }
    
    /* Initialize latch data and update hardware */
    au8LatchData[0] = 0;
    au8LatchData[1] = 0;
    vidLED_LatchUpdate(0);
    vidLED_LatchUpdate(1);
}

void LEDController_vidSetState(uint8_t u8LedId, LEDState_t enuState)
{
    enuLedStatus[u8LedId].enuState = enuState;
    enuLedStatus[u8LedId].enuPattern = LED_PATTERN_NONE;
    
    if(enuState == LED_STATE_ON) {
        vidLED_UpdateHardware(u8LedId, True);
    }
    else if(enuState == LED_STATE_OFF) {
        vidLED_UpdateHardware(u8LedId, False);
    }
}

void LEDController_vidSetPattern(uint8_t u8LedId, LEDPattern_t enuPattern)
{
    enuLedStatus[u8LedId].enuPattern = enuPattern;
    enuLedStatus[u8LedId].enuState = LED_STATE_BLINKING;
    enuLedStatus[u8LedId].u16LastToggleTime = u16LED_GetCurrentTime();
}

void LEDController_vidProcess(void)
{
    uint8_t u8I;
    uint16_t u16CurrentTime;
    uint16_t u16OnTime, u16OffTime;
    
    u16CurrentTime = u16LED_GetCurrentTime();
    
    for(u8I = 0; u8I < cu8LED_MAX_COUNT; u8I++) {
        if(enuLedStatus[u8I].enuState == LED_STATE_BLINKING) {
            /* Determine pattern timing */
            switch(enuLedStatus[u8I].enuPattern) {
                case LED_PATTERN_INTERNAL_CALL:
                    u16OnTime = cu16LED_INTERNAL_CALL_ON_TIME;
                    u16OffTime = cu16LED_INTERNAL_CALL_OFF_TIME;
                    break;
                    
                case LED_PATTERN_EXTERNAL_CALL:
                    u16OnTime = cu16LED_EXTERNAL_CALL_ON_TIME;
                    u16OffTime = cu16LED_EXTERNAL_CALL_OFF_TIME;
                    break;
                    
                case LED_PATTERN_MAINTENANCE:
                    u16OnTime = cu16LED_MAINTENANCE_ON_TIME;
                    u16OffTime = cu16LED_MAINTENANCE_OFF_TIME;
                    break;
                    
                case LED_PATTERN_ERROR:
                    u16OnTime = cu16LED_ERROR_ON_TIME;
                    u16OffTime = cu16LED_ERROR_OFF_TIME;
                    break;
                    
                default:
                    continue;
            }
            
            /* Check if it's time to toggle */
            if(enuLedStatus[u8I].bCurrentOutput == True) {
                if((u16CurrentTime - enuLedStatus[u8I].u16LastToggleTime) >= u16OnTime) {
                    enuLedStatus[u8I].bCurrentOutput = False;
                    enuLedStatus[u8I].u16LastToggleTime = u16CurrentTime;
                    vidLED_UpdateHardware(u8I, False);
                }
            }
            else {
                if((u16CurrentTime - enuLedStatus[u8I].u16LastToggleTime) >= u16OffTime) {
                    enuLedStatus[u8I].bCurrentOutput = True;
                    enuLedStatus[u8I].u16LastToggleTime = u16CurrentTime;
                    vidLED_UpdateHardware(u8I, True);
                }
            }
        }
    }
}