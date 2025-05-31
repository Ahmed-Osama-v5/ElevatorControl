/* ************************************************************************ */
/* ************************************************************************ */
/*                        (C) Copyright 2025 by                             */
/*                              HexaMix                                     */
/*                         All rights reserved                              */
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

#include "SensorManager.h"
#include "SystemConfig.h"
#define ButtonDriver_C

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "Std_Types.h"

/* system and library inclusions ****************************************** */

#include <util/delay.h>

/* other components of the project **************************************** */

#include "dio.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "ButtonDriver.h"

/* access to the modul's local configuration */
#include "ButtonDriver_lcfg.h"



/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/* Initialise the component. */

void ButtonDriver_vidInit (void)
{
    /* Do nothing */
}

/* Main Functions ********************************************************* */
boolean Button_bIsPressed(ButtonId_t enuButtonId)
{
    /* Return value variable */
    boolean bBtnPressed = False;
    
    /* Sensor state variable */
    uint8_t* pu8SensorState = NULL;

    switch (enuButtonId)
    {
        case BTN_UP:
            // Select CALL_9 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_HIGH)
            {
                bBtnPressed = True; // Button is pressed
            }
            else
            {
                bBtnPressed = False; // Button is not pressed
            }
            break;
        case BTN_DN:
            // Select CALL_10 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_HIGH)
            {
                bBtnPressed = True; // Button is pressed
            }
            else
            {
                bBtnPressed = False; // Button is not pressed
            }
            break;
        case BTN_OK:
            // Select CALL_8 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_HIGH)
            {
                bBtnPressed = True; // Button is pressed
            }
            else
            {
                bBtnPressed = False; // Button is not pressed
            }
            break;
        case BTN_PRG:
            (void) SensorManager_stdReadSensor(PHASE_2, pu8SensorState);
            if(pu8SensorState != NULL && *pu8SensorState == cu8SENSOR_STATE_ACTIVE)
            {
                bBtnPressed = True; // Button is pressed
            }
            else
            {
                bBtnPressed = False; // Button is not pressed
            }
            break;
        default:
            bBtnPressed = False; // Invalid button ID
            break;
    }
    _delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
    return bBtnPressed;
}
/* ************************************************************************ */

boolean Button_bIsReleased(ButtonId_t enuButtonId)
{
    /* Return value variable */
    boolean bBtnReleased = False;
    
    /* Sensor state variable */
    uint8_t* pu8SensorState = NULL;

    switch (enuButtonId)
    {
        case BTN_UP:
            // Select CALL_9 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_HIGH)
            {
                bBtnReleased = True; // Button is released
            }
            else
            {
                bBtnReleased = False; // Button is not released
            }
            break;
        case BTN_DN:
            // Select CALL_10 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_HIGH)
            {
                bBtnReleased = True; // Button is released
            }
            else
            {
                bBtnReleased = False; // Button is not released
            }
            break;
        case BTN_OK:
            // Select CALL_8 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_HIGH)
            {
                bBtnReleased = True; // Button is released
            }
            else
            {
                bBtnReleased = False; // Button is not released
            }
            break;
        case BTN_PRG:
            (void) SensorManager_stdReadSensor(PHASE_2, pu8SensorState);
            if(pu8SensorState != NULL && *pu8SensorState == cu8SENSOR_STATE_INACTIVE)
            {
                bBtnReleased = True; // Button is released
            }
            else
            {
                bBtnReleased = False; // Button is not released
            }
            break;
        default:
            bBtnReleased = False; // Invalid button ID
            break;
    }
    _delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
    return bBtnReleased;
}
/* ************************************************************************ */

/* ************************************************************************ */

/* ************************************************************************ */
/* Xxx   Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/*  */


/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */