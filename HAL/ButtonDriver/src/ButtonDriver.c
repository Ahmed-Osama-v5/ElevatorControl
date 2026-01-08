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
#include "SystemConfig.h"

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
    Dio_Cfg_t stDioCfg;
    /* Initialize the DIO configuration structure */
    stDioCfg.enuPinDir = DIR_INPUT; // Set pin direction to input
    stDioCfg.enuGPIO = enuBTN_INPUT_GPIO; // Set the GPIO port
    stDioCfg.u8Pin = cu8BTN_INPUT_PIN; // Set the pin number
    /* Initialize the DIO pin */
    DIO_Init(&stDioCfg);
}

/* Main Functions ********************************************************* */
boolean ButtonDriver_bIsPressed(ButtonId_t enuButtonId)
{
    /* Return value variable */
    boolean bBtnPressed = FALSE;
    
    /* Sensor state variable */
    PinState_t enuSensorState = STATE_LOW;

    switch (enuButtonId)
    {
        case BTN_UP:
            // Select CALL_9 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_LOW)
            {
                bBtnPressed = TRUE; // Button is pressed
            }
            else
            {
                bBtnPressed = FALSE; // Button is not pressed
            }
            _delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
            break;
        case BTN_DN:
            // Select CALL_10 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_LOW)
            {
                bBtnPressed = TRUE; // Button is pressed
            }
            else
            {
                bBtnPressed = FALSE; // Button is not pressed
            }
            _delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
            break;
        case BTN_OK:
            // Select CALL_8 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) == STATE_LOW)
            {
                bBtnPressed = TRUE; // Button is pressed
            }
            else
            {
                bBtnPressed = FALSE; // Button is not pressed
            }
            break;
        case BTN_PRG:
            (void) SensorManager_stdReadSensor(PHASE_2, &enuSensorState);
            if(enuSensorState == cenuSENSOR_STATE_ACTIVE)
            {
                bBtnPressed = TRUE; // Button is pressed
            }
            else
            {
                bBtnPressed = FALSE; // Button is not pressed
            }
            break;
        default:
            bBtnPressed = FALSE; // Invalid button ID
            break;
    }
    //_delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
    return bBtnPressed;
}
/* ************************************************************************ */

boolean ButtonDriver_bIsReleased(ButtonId_t enuButtonId)
{
    /* Return value variable */
    boolean bBtnReleased = FALSE;
    
    /* Sensor state variable */
    PinState_t enuSensorState = STATE_LOW;

    switch (enuButtonId)
    {
        case BTN_UP:
            // Select CALL_9 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_LOW)
            {
                bBtnReleased = TRUE; // Button is released
            }
            else
            {
                bBtnReleased = FALSE; // Button is not released
            }
            break;
        case BTN_DN:
            // Select CALL_10 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_LOW)
            {
                bBtnReleased = TRUE; // Button is released
            }
            else
            {
                bBtnReleased = FALSE; // Button is not released
            }
            break;
        case BTN_OK:
            // Select CALL_8 input
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1); // Wait for the sensor to stabilize
            // Read the sensor state
            if(DIO_ReadPin(enuBTN_INPUT_GPIO, cu8BTN_INPUT_PIN) != STATE_LOW)
            {
                bBtnReleased = TRUE; // Button is released
            }
            else
            {
                bBtnReleased = FALSE; // Button is not released
            }
            break;
        case BTN_PRG:
            (void) SensorManager_stdReadSensor(PHASE_2, &enuSensorState);
            if(enuSensorState == cenuSENSOR_STATE_INACTIVE)
            {
                bBtnReleased = TRUE; // Button is released
            }
            else
            {
                bBtnReleased = FALSE; // Button is not released
            }
            break;
        default:
            bBtnReleased = FALSE; // Invalid button ID
            break;
    }
    //_delay_ms(cu8BUTTON_DEFAULT_DEBOUNCE_TIME); // Debounce delay
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