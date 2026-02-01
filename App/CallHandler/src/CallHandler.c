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

#include "CallHandler_gcfg.h"
#define CallHandler_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "std_types.h"

/* other components of the project **************************************** */

#include "dio.h"
#include "ElevatorController.h"
#include "LEDController.h"
#include "RelayManager.h"

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

/* Elevator object pointer */
static Elevator_t* spstrElevator;

/* Calls scanner */
/**
 * @brief Get the Call object
 * 
 * @param enuCallType 
 * @return uint16_t 
 */
static uint16_t u16GetCall(CallType_t enuCallType);

/**
 * @brief Process the call queue
 * @param bIsQueueEmpty Indicates if the call queue is empty
 */
static void vidProcessCallQueue(void);

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
void CallHandler_vidInit(Elevator_t* pstrElevator)
{
    if(pstrElevator != NULL)
    {
        uint8_t u8Index = 0;

        spstrElevator = pstrElevator;
        
        /* Init call queue */
        for (u8Index = 0; u8Index < spstrElevator->u8FloorCount; u8Index++)
        {
            spstrElevator->aenuFloorCalls[u8Index] = CALL_NONE;
        }
    }
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
void CallHandler_vidGetCall(void)
{
    // Implementation to scan call inputs and register a call
    if (spstrElevator != NULL)
    {
        uint16_t u16CallResult = 0U;
        uint8_t u8FloorIndex = 0U;
        uint8_t u8CurrentFloor = ElevatorController_u8GetCurrentFloor();

        /* TODO: remember to turn all LEDs off through "ElevatorController" */

        /* Scan Inner calls */
        u16CallResult = u16GetCall(CALL_INTERNAL);

        /* Process result */
        if(u16CallResult != 0U)
        {
            for(u8FloorIndex = 0U; u8FloorIndex < spstrElevator->u8FloorCount; u8FloorIndex++)
            {
                if (u16CallResult & (1U << u8FloorIndex))
                {
                    if(spstrElevator->aenuFloorCalls[u8FloorIndex] == CALL_EXTERNAL)
                    {
                        spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_INTERNAL;

                        /* Set led pattern to internal */
                        LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_INTERNAL_CALL);
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
                            spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_INTERNAL;

                            /* Set led pattern to internal */
                            LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_INTERNAL_CALL);

                            if(spstrElevator->enuDirection == DIR_IDLE)
                            {
                                spstrElevator->u8DestinationFloor = u8FloorIndex;
                                if(spstrElevator->u8CurrentFloor < u8FloorIndex)
                                    spstrElevator->enuDirection = DIR_UP;
                                else
                                    spstrElevator->enuDirection = DIR_DOWN;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            u16CallResult = 0U;

            /* Scan Outer calls */
            u16CallResult = u16GetCall(CALL_EXTERNAL);
            
            /* Process result */
            if(u16CallResult != 0U)
            {
                for(u8FloorIndex = 0U; u8FloorIndex < spstrElevator->u8FloorCount; u8FloorIndex++)
                {
                    if (u16CallResult & (1U << u8FloorIndex))
                    {
                        /* Check if call is regestered */
                        if(spstrElevator->aenuFloorCalls[u8FloorIndex] != CALL_NONE)
                        {
                            /* Call already registered, discard new call */
                        }

                        /* Handle new call */
                        else
                        {
                            /* check if call is made to current floor from external */
                            if((u8FloorIndex == spstrElevator->u8CurrentFloor) && (spstrElevator->enuDirection == DIR_IDLE))
                            {
                                /* Turn on cabin lights */
                                RelayManager_vidActivateRelay(RELAY_LIGHT);

                                /* Activate cabin lights timeout */
                                ElevatorController_vidStartLightTimer();
                            }
                            /* Check if call is equal to current floor and discard it */
                            if((u8FloorIndex == spstrElevator->u8CurrentFloor)
                            ||((spstrElevator->enuDirection != DIR_IDLE) && ((u8FloorIndex == (spstrElevator->u8CurrentFloor - 1)) || (u8FloorIndex == (spstrElevator->u8CurrentFloor + 1))) && (ElevatorController_u8GetSelectorCnt() % 2)))
                            {
                                /* Call is either to current floor or elevator is too close to the called floor */
                                /* Don't register the call */
                            }
                            else
                            {
                                /* New valid call */
                                spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_EXTERNAL;

                                /* Set led pattern to internal */
                                LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_EXTERNAL_CALL);

                                if(spstrElevator->enuDirection == DIR_IDLE)
                                {
                                    spstrElevator->u8DestinationFloor = u8FloorIndex;
                                    if(spstrElevator->u8CurrentFloor < u8FloorIndex)
                                        spstrElevator->enuDirection = DIR_UP;
                                    else
                                        spstrElevator->enuDirection = DIR_DOWN;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(spstrElevator->enuDirection == DIR_STOPPING)
        {
            vidProcessCallQueue();
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
 * @brief Get the Call request from hardware
 * 
 * @param enuCallType 
 * @return uint16_t 
 */
static uint16_t u16GetCall(CallType_t enuCallType)
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
        switch(u8FloorIndex)
        {
            case 0:
                // Handle floor 0
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 1:
                // Handle floor 1
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 2:
                // Handle floor 2
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 3:
                // Handle floor 3
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 4:
                // Handle floor 4
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 5:
                // Handle floor 5
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 6:
                // Handle floor 6
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 7:
                // Handle floor 7
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
        }
        
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
        switch(u8FloorIndex)
        {
            case 0:
                // Handle floor 8
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 1:
                // Handle floor 9
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 2:
                // Handle floor 10
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 3:
                // Handle floor 11
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_LOW);
                break;
            case 4:
                // Handle floor 12
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 5:
                // Handle floor 13
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 6:
                // Handle floor 14
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_LOW);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
            case 7:
                // Handle floor 15
                DIO_WritePin(cstrCallDio_Config.enuSEL_0_Port, cstrCallDio_Config.u8SEL_0_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_1_Port, cstrCallDio_Config.u8SEL_1_Pin, STATE_HIGH);
                DIO_WritePin(cstrCallDio_Config.enuSEL_2_Port, cstrCallDio_Config.u8SEL_2_Pin, STATE_HIGH);
                break;
        }
        
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


/**
 * @brief Process the call queue
 * @param bIsQueueEmpty Indicates if the call queue is empty
 */
static void vidProcessCallQueue(void)
{
	uint8_t tmpFlag = 0, retVal = 0, i;
    if(spstrElevator->enuDirection == DIR_UP)
    {
        for(i=spstrElevator->u8CurrentFloor;i<spstrElevator->u8FloorCount;i++)
        {
            if((i > spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[i] == CALL_INTERNAL))
            {
				tmpFlag = 1;
                retVal = i;
				break;
            }
            else
			{
				/* Do nothing */
			}
        }
		if(! tmpFlag)
		{
			for(i=spstrElevator->u8CurrentFloor;i<spstrElevator->u8FloorCount;i++)
			{
				if((i > spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[i] != CALL_NONE))
				{
					tmpFlag = 1;
					retVal = i;
				}
				else
				{
					/* Do nothing */
				}
			}
			if(tmpFlag)
				spstrElevator->u8DestinationFloor =  retVal;
		}
		else
		{
			spstrElevator->u8DestinationFloor =  retVal;
		}

        /* Check if destination is down */
        for(i=spstrElevator->u8CurrentFloor;i>0;i--)
        {
            if((i < spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[i] != CALL_NONE))
            {
                spstrElevator->enuDirection = DIR_DOWN;
                spstrElevator->u8DestinationFloor =  i;
            }
        }
        if((0 < spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[0] != CALL_NONE))
        {
            spstrElevator->enuDirection = DIR_DOWN;
            spstrElevator->u8DestinationFloor =  0;
        }
    }
    else
    {
        /* Check if destination is down */
        for(i=spstrElevator->u8CurrentFloor;i>0;i--)
        {
            if((i < spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[i] != CALL_NONE))
            {
                spstrElevator->u8DestinationFloor =  i;
            }
        }
        if((0 < spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[0] != CALL_NONE))
        {
            spstrElevator->enuDirection = DIR_DOWN;
            spstrElevator->u8DestinationFloor =  0;
        }

        for(i=spstrElevator->u8CurrentFloor;i<spstrElevator->u8FloorCount;i++)
        {
            if((i > spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[i] != CALL_NONE))
            {
                spstrElevator->enuDirection = DIR_UP;
                spstrElevator->u8DestinationFloor =  i;
            }
        }
    }

    #if 0
    /* Process call queue and set destination floor */
    uint8_t u8Index = 0;
    for(u8Index=0;u8Index<spstrElevator->u8FloorCount;u8Index++)
    {
        if((u8Index > spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[u8Index] != CALL_NONE))
        {
            /* Check collection direction */
            if(spstrElevator->enuCollDir == COLLECTION_UP)
            {
                /* Check motion direction */
                if(spstrElevator->enuDirection == DIR_UP)
                {
                    spstrElevator->u8DestinationFloor = u8Index;
                    break;
                }
            }
            else
            {
                /* Collection down */

                /* Check motion direction */
                if(spstrElevator->enuDirection == DIR_UP)
                {
                    /* If call is internal */
                    if(spstrElevator->aenuFloorCalls[u8Index] == CALL_INTERNAL)
                    {
                        spstrElevator->u8DestinationFloor = u8Index;
                        break;
                    }
                }
                else
                {
                    /* Do nothing */
                }
            }
        }
        else if((u8Index < spstrElevator->u8CurrentFloor) && (spstrElevator->aenuFloorCalls[u8Index] != CALL_NONE))
        {
            /* Check collection direction */
            if(spstrElevator->enuCollDir == COLLECTION_UP)
            {
                /* Check motion direction */
                if(spstrElevator->enuDirection == DIR_DOWN)
                {
                    /* If call is internal */
                    if(spstrElevator->aenuFloorCalls[u8Index] == CALL_INTERNAL)
                    {
                        spstrElevator->u8DestinationFloor = u8Index;
                        break;
                    }
                }
            }
            else
            {
                /* Collection down */

                /* Check motion direction */
                if(spstrElevator->enuDirection == DIR_DOWN)
                {
                    spstrElevator->u8DestinationFloor = u8Index;
                    break;
                }
                else
                {
                    /* Do nothing */
                }
            }
        }
        else
        {
            /* Do nothing */
        }
    }
        #endif
}
/* ************************************************************************ */


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
