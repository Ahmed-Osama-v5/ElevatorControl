
/* ************************************************************************ */
/* ************************************************************************ */
/*                        (C) Copyright 2026 by                             */
/*                                 HexaMix                                  */
/*                         All rights reserved                              */
/*           This software is the exclusive property of HexaMix.            */
/*   It cannot be communicated or divulged to anybody without a previous    */
/*  written authorisation.                                                  */
/*                                                                          */
/*     Any partial or complete copy of this program whether it is in its    */
/*   original shape or in its modified shape has to mention this copyright  */
/*  and its proprietor.                                                     */
/*                                                                          */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                        FILE INCLUSION MANAGEMENT                         */
/* ************************************************************************ */
/* ************************************************************************ */
/* Set Ownership                                                            */
/* ************************************************************************ */

#define LEDController_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "Std_Types.h"

/* other components of the project **************************************** */


/* own header inclusions ************************************************** */

/* access to its own exports */
#include "LEDController.h"

/* access to the modul's local configuration */
#include "LEDController_lcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */

/* Module private variables */
static LEDStatus_t enuLedStatus[cu8MAX_FLOORS];
static uint8_t au8LatchData[2] = {0};  // Data for two 74HC573 latches
static boolean bModuleInitialized = FALSE;

/* Private function declarations */
static boolean bLED_ValidateId(uint8_t u8LedId);
static void vidLED_UpdateHardware(uint8_t u8LedId, boolean bState);
static uint16_t u16LED_GetCurrentTime(void);
static void vidLED_LatchUpdate(uint8_t u8LatchIndex);

/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/* Initialise the component. */
/**
 * @brief Initializes the LED Controller module.
 * 
 */
void LEDController_vidInit(void)
{
    uint8_t u8I;

    /* Initialize latch control pins */
    Dio_Cfg_t dio_config = {
        .enuGPIO = cenuLED_LATCH0_PORT,
        .u8Pin = cu8LED_LATCH0_PIN,
        .enuPinDir = DIR_OUTPUT
    };
    DIO_Init(&dio_config);
    
    dio_config.enuGPIO = cenuLED_LATCH1_PORT;
    dio_config.u8Pin = cu8LED_LATCH1_PIN;
    DIO_Init(&dio_config);

    /* Initialize data port */
    dio_config.enuGPIO = cenuLED_DATA_PORT;
    dio_config.u8Pin = 0xFF;  // All pins as output
    DIO_Init(&dio_config);
    
    /* Initialize LED states */
    for(u8I = 0; u8I < cu8MAX_FLOORS; u8I++) {
        enuLedStatus[u8I].enuState = LED_STATE_OFF;
        enuLedStatus[u8I].enuPattern = LED_PATTERN_NONE;
        enuLedStatus[u8I].u16LastToggleTime = 0;
        enuLedStatus[u8I].bCurrentOutput = FALSE;
        enuLedStatus[u8I].bIsInitialized = TRUE;
        
        vidLED_UpdateHardware(u8I, FALSE);
    }
    
    /* Initialize latch data and update hardware */
    au8LatchData[0] = 0;
    au8LatchData[1] = 0;
    vidLED_LatchUpdate(0);
    vidLED_LatchUpdate(1);
}

/**
 * @brief Sets the state of a specific LED.
 * 
 * @param u8LedId 
 * @param enuState 
 */
void LEDController_vidSetState(uint8_t u8LedId, LEDState_t enuState)
{
    enuLedStatus[u8LedId].enuState = enuState;
    enuLedStatus[u8LedId].enuPattern = LED_PATTERN_NONE;
    
    if(enuState == LED_STATE_ON) {
        vidLED_UpdateHardware(u8LedId, TRUE);
    }
    else if(enuState == LED_STATE_OFF) {
        vidLED_UpdateHardware(u8LedId, FALSE);
    }
}

/**
 * @brief Sets the pattern of a specific LED
 * 
 * @param u8LedId 
 * @param enuPattern 
 */
void LEDController_vidSetPattern(uint8_t u8LedId, LEDPattern_t enuPattern)
{
    enuLedStatus[u8LedId].enuPattern = enuPattern;
    enuLedStatus[u8LedId].enuState = LED_STATE_BLINKING;
    enuLedStatus[u8LedId].u16LastToggleTime = u16LED_GetCurrentTime();
}

/**
 * @brief Processes the LED states and patterns
 * 
 */
void LEDController_vidProcess(void)
{
    uint8_t u8I;
    uint16_t u16CurrentTime;
    uint16_t u16OnTime, u16OffTime;
    
    u16CurrentTime = u16LED_GetCurrentTime();
    
    for(u8I = 0; u8I < cu8MAX_FLOORS; u8I++) {
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
            if(enuLedStatus[u8I].bCurrentOutput == TRUE) {
                if((u16CurrentTime - enuLedStatus[u8I].u16LastToggleTime) >= u16OnTime) {
                    enuLedStatus[u8I].bCurrentOutput = FALSE;
                    enuLedStatus[u8I].u16LastToggleTime = u16CurrentTime;
                    vidLED_UpdateHardware(u8I, FALSE);
                }
            }
            else {
                if((u16CurrentTime - enuLedStatus[u8I].u16LastToggleTime) >= u16OffTime) {
                    enuLedStatus[u8I].bCurrentOutput = TRUE;
                    enuLedStatus[u8I].u16LastToggleTime = u16CurrentTime;
                    vidLED_UpdateHardware(u8I, TRUE);
                }
            }
        }
    }
}
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */
static void vidLED_LatchUpdate(uint8_t u8LatchIndex) {
    // Output the data to the data port
    DIO_WritePort(cenuLED_DATA_PORT, au8LatchData[u8LatchIndex]);
    // Toggle the corresponding latch pin
    if (u8LatchIndex == 0) {
        DIO_WritePin(cenuLED_LATCH0_PORT, cu8LED_LATCH0_PIN, STATE_HIGH);
        // Small delay if needed
        DIO_WritePin(cenuLED_LATCH0_PORT, cu8LED_LATCH0_PIN, STATE_LOW);
    } else {
        DIO_WritePin(cenuLED_LATCH1_PORT, cu8LED_LATCH1_PIN, STATE_HIGH);
        // Small delay if needed
        DIO_WritePin(cenuLED_LATCH1_PORT, cu8LED_LATCH1_PIN, STATE_LOW);
    }
}

static void vidLED_UpdateHardware(uint8_t u8LedId, boolean bState) {
    uint8_t u8LatchIndex = LED_HWConfig[u8LedId].u8LatchIndex;
    uint8_t u8BitPos = LED_HWConfig[u8LedId].u8BitPos;
    
    if(bLED_ValidateId(u8LedId))
    {
        if(bState)
        {
            if(u8LedId >= 8U)
            {
                au8LatchData[u8LatchIndex] |= (1 << u8BitPos);
            }
            else
            {
                au8LatchData[u8LatchIndex] |= (1 << (7 - u8BitPos));
            }
        }
        else
        {
            if(u8LedId >= 8U)
            {
                au8LatchData[u8LatchIndex] &= ~(1 << u8BitPos);
            }
            else
            {
                au8LatchData[u8LatchIndex] &= ~(1 << (7 - u8BitPos));
            }
        }
        vidLED_LatchUpdate(u8LatchIndex);
    }
}

static boolean bLED_ValidateId(uint8_t u8LedId)
{
    if(u8LedId >= cu8MAX_FLOORS) {
        return FALSE;
    }
    return TRUE;
}

static uint16_t u16LED_GetCurrentTime(void) {
    /* This should be implemented based on your system's timing service */
    return Timer_GetValue(TIMER_CH1);
}
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
