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

#define CallHandler_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "std_types.h"

/* other components of the project **************************************** */

#include "dio.h"
#include "ElevatorController.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "CallHandler.h"

/* access to the modul's local configuration */
#include "CallHandler_lcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */

/* Calls scanner */
/**
 * @brief Get the Call object
 * 
 * @param enuCallType 
 * @return uint8_t 
 */
static uint8_t u8GetCall(CallType_t enuCallType);

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
 * @brief This function initializes the CallHandler component.
 * 
 */
void CallHandler_vidInit(void)
{
    /* Initialize Call inputs DIOs */
    Dio_Cfg_t dio_config = {
        .enuGPIO = cstrCallDio_Config.enuCALL_0_Port,
        .u8Pin = cstrCallDio_Config.u8CALL_0_Pin,
        .enuPinDir = DIR_INPUT
    };
    DIO_Init(&dio_config);

    dio_config.enuGPIO = cstrCallDio_Config.enuCALL_1_Port;
    dio_config.u8Pin = cstrCallDio_Config.u8CALL_1_Pin;
    DIO_Init(&dio_config);

    dio_config.enuGPIO = cstrCallDio_Config.enuCALL_CON_Port;
    dio_config.u8Pin = cstrCallDio_Config.u8CALL_CON_Pin;
    dio_config.enuPinDir = DIR_OUTPUT;
    DIO_Init(&dio_config);
}

/* ************************************************************************ */
/* Call Services                                                            */
/* ************************************************************************ */
/**
 * @brief scans call inputs and register a call in the output param.
 * 
 * @param pstrCall struct holding call info.
 */
void CallHandler_vidGetCall(Call_t* cpstrCall)
{
    // Implementation to scan call inputs and register a call
    if (cpstrCall != NULL)
    {
        uint16_t u16CallResult = 0U;
        uint8_t u8FloorIndex = 0U;
        uint8_t u8CurrentFloor = ElevatorController_u8GetCurrentFloor();

        /* TODO: remember to turn all LEDs off through "ElevatorController" */

        /* Scan Inner calls */
        u16CallResult = u8GetCall(CALL_INTERNAL);

        /* Process result */
        if(u16CallResult != 0U)
        {
            for(u8FloorIndex = 0U; u8FloorIndex < cu8MAX_FLOORS; u8FloorIndex++)
            {
                if (u16CallResult & (1U << u8FloorIndex))
                {
                    /* Check if call is regestered */
                    if(cpstrCall->u8Floor[u8FloorIndex] == 1)
                    {
                        cpstrCall->enuCallType[u8FloorIndex] = CALL_INTERNAL;
                    }

                    /* Handle new call */
                    else
                    {
                        /* Check if call is equal to current floor and discard it */
                        if((u8FloorIndex == u8CurrentFloor)
                         ||(((u8FloorIndex == (u8CurrentFloor - 1)) || (u8FloorIndex == (u8CurrentFloor + 1))) && (ElevatorController_u8GetSelectorCnt() % 2)))
                        {
                            /* Call is either to current floor or elevator is too close to the called floor */
                            /* Don't register the call */
                        }
                        else
                        {
                            /* New valid call */
                            cpstrCall->u8Floor[u8FloorIndex] = 1;
                            cpstrCall->enuCallType[u8FloorIndex] = CALL_INTERNAL;
                        }
                    }
                }
            }
        }
        else
        {
            u16CallResult = 0U;

            /* Scan Outer calls */
            u16CallResult = u8GetCall(CALL_EXTERNAL);
            
            /* Process result */
            if(u16CallResult != 0U)
            {
                for(u8FloorIndex = 0U; u8FloorIndex < cu8MAX_FLOORS; u8FloorIndex++)
                {
                    if (u16CallResult & (1U << u8FloorIndex))
                    {
                        /* Check if call is regestered */
                        if(cpstrCall->u8Floor[u8FloorIndex] == 1)
                        {
                            /* Call already registered, discard new call */
                        }

                        /* Handle new call */
                        else
                        {
                            /* Check if call is equal to current floor and discard it */
                            if((u8FloorIndex == u8CurrentFloor)
                            ||(((u8FloorIndex == (u8CurrentFloor - 1)) || (u8FloorIndex == (u8CurrentFloor + 1))) && (ElevatorController_u8GetSelectorCnt() % 2)))
                            {
                                /* Call is either to current floor or elevator is too close to the called floor */
                                /* Don't register the call */
                            }
                            else
                            {
                                /* New valid call */
                                cpstrCall->u8Floor[u8FloorIndex] = 1;
                                cpstrCall->enuCallType[u8FloorIndex] = CALL_EXTERNAL;
                            }
                        }
                    }
                }
            }
        }

    }
    else
    {
        // Handle the case where cpstrCall is NULL, e.g., log an error or take appropriate action
    }
}
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */
/* Calls scanner */
/**
 * @brief Get the Call object
 * 
 * @param enuCallType 
 * @return uint8_t 
 */
static uint8_t u8GetCall(CallType_t enuCallType)
{
    PinState_t enuPinState = STATE_HIGH;
	uint16_t u16RetVal = 0U;
    uint8_t u8FloorIndex = 0U;

    if(enuCallType == CALL_INTERNAL)
    {
        /* Set CallControl pin to Inner call */
        DIO_WritePin(cstrCallDio_Config.enuCALL_CON_Port, cstrCallDio_Config.u8CALL_CON_Pin, STATE_LOW);
        
    }
    else if (enuCallType == CALL_EXTERNAL)
    {
        /* Set CallControl pin to Outer call */
        DIO_WritePin(cstrCallDio_Config.enuCALL_CON_Port, cstrCallDio_Config.u8CALL_CON_Pin, STATE_HIGH);
    }
    else
    {
        // Handle other call types if necessary or log an error
    }

    /* Scan floors 0 to 7 */
    for(u8FloorIndex=0;u8FloorIndex<8;u8FloorIndex++)
    {
        /* Mux select(u8FloorIndex) */
        
        /* Read floor inner call from mux output */
        enuPinState = DIO_ReadPin(cstrCallDio_Config.enuCALL_0_Port, cstrCallDio_Config.u8CALL_0_Pin);

        if(enuPinState == STATE_LOW)
        {
            u16RetVal |= (1U << u8FloorIndex);
        }
        else
        {
            u16RetVal &= ~(1U << u8FloorIndex);
        }
    }
    /* Scan floors 8 to 15 */
    for(u8FloorIndex=0;u8FloorIndex<8;u8FloorIndex++)
    {
        /* Mux select(7-u8FloorIndex) */
        
        /* Read floor inner call from mux output */
        enuPinState = DIO_ReadPin(cstrCallDio_Config.enuCALL_1_Port, cstrCallDio_Config.u8CALL_1_Pin);

        if(enuPinState == STATE_LOW)
        {
            u16RetVal |= (1U << (u8FloorIndex + 8));
        }
        else
        {
            u16RetVal &= ~(1U << (u8FloorIndex + 8));
        }
    }

    return u16RetVal;
}
/* ************************************************************************ */


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
