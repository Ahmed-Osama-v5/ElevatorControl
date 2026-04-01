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
#include "ElevatorController_gcfg.h"
#include "Std_Types.h"
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
#include "debug_log.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "CallHandler.h"

/* access to the modul's local configuration */
#include "CallHandler_lcfg.h"

/* ************************************************************************ */

#define FLOOR_IS_CLOSE_WHILE_MOVING(floor, currentFloor) \
    (((floor) == ((currentFloor) - 1U) || (floor) == ((currentFloor) + 1U)) \
     && (ElevatorController_u8GetSelectorCnt() % 2U))

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
    /* Phase 1 — Call Registration */
    if (spstrElevator != NULL)
    {
        uint16_t u16CallResult = 0U;
        uint8_t u8FloorIndex = 0U;

        /* Scan Inner calls */
        u16CallResult = u16GetCall(CALL_INTERNAL);

        /* Process result */
        if(u16CallResult != 0U)
        {
            DBG_PRINT_STRING("Processing inner call");
            for(u8FloorIndex = 0U; u8FloorIndex < spstrElevator->u8FloorCount; u8FloorIndex++)
            {
                if (u16CallResult & (1U << u8FloorIndex))
                {
                    /* 
                        if floor == currentFloor → discard (already there)
                        if floor is proximity-close AND elevator moving → discard
                        if already CALL_INTERNAL → discard (already registered)
                    */
                    if((u8FloorIndex == spstrElevator->u8CurrentFloor)
                        || ((spstrElevator->enuDirection != DIR_IDLE) && FLOOR_IS_CLOSE_WHILE_MOVING(u8FloorIndex, spstrElevator->u8CurrentFloor))
                        || (spstrElevator->aenuFloorCalls[u8FloorIndex] == CALL_INTERNAL))
                    {
                        /* Don't register the call */
                        DBG_PRINT_STRING("inner Call already registered / current floor / close to current floor");
                    }
                    /* if currently CALL_EXTERNAL → upgrade to CALL_INTERNAL, update LED */
                    else if(spstrElevator->aenuFloorCalls[u8FloorIndex] == CALL_EXTERNAL)
                    {
                        DBG_PRINT_STRING("switched call from external to internal");

                        spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_INTERNAL;

                        /* Set led pattern to internal */
                        LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_INTERNAL_CALL);
                        
                    }
                    /* else → register CALL_INTERNAL, update LED */
                    else
                    {
                        /* New valid call */
                        DBG_PRINT_STRING("New inner call");

                        spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_INTERNAL;

                        /* Set led pattern to internal */
                        LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_INTERNAL_CALL);

                        if(spstrElevator->enuDirection == DIR_IDLE)
                        {
                            spstrElevator->u8DestinationFloor = u8FloorIndex;
                            if(spstrElevator->u8CurrentFloor < u8FloorIndex)
                            {
                                spstrElevator->enuDirection = DIR_UP;
                                spstrElevator->enuLastTravelDir = DIR_UP;
                            }
                            else
                            {
                                spstrElevator->enuDirection = DIR_DOWN;
                                spstrElevator->enuLastTravelDir = DIR_DOWN;
                            }
                        }
                    }
                }
            }
        }
        u16CallResult = 0U;

        /* Scan Outer calls */
        u16CallResult = u16GetCall(CALL_EXTERNAL);
        
        /* Process result */
        if(u16CallResult != 0U)
        {
            DBG_PRINT_STRING("Processing external call");
            for(u8FloorIndex = 0U; u8FloorIndex < spstrElevator->u8FloorCount; u8FloorIndex++)
            {
                /*
                    if floor == currentFloor AND DIR_IDLE → activate light, don't register
                    if floor == currentFloor → discard
                    if proximity-close AND elevator moving → discard
                    if aenuFloorCalls[floor] != CALL_NONE → discard (already registered)
                    else → register CALL_EXTERNAL, update LED
                */
                if (u16CallResult & (1U << u8FloorIndex))
                {
                    /* if floor == currentFloor AND DIR_IDLE → activate light, don't register */
                    if((u8FloorIndex == spstrElevator->u8CurrentFloor) && (spstrElevator->enuDirection == DIR_IDLE))
                    {
                        DBG_PRINT_STRING("Turn on cabin light");
                        /* Turn on cabin lights */
                        RelayManager_vidActivateRelay(RELAY_LIGHT);

                        /* Activate cabin lights timeout */
                        ElevatorController_vidStartLightTimer();
                    }
                    /* 
                        if floor == currentFloor → discard
                        if proximity-close AND elevator moving → discard
                        if aenuFloorCalls[floor] != CALL_NONE → discard (already registered)
                    */
                    else if((u8FloorIndex == spstrElevator->u8CurrentFloor)
                    ||FLOOR_IS_CLOSE_WHILE_MOVING(u8FloorIndex, spstrElevator->u8CurrentFloor)
                    || (spstrElevator->aenuFloorCalls[u8FloorIndex] != CALL_NONE))
                    {
                        /* Call is either to current floor or elevator is too close to the called floor */
                        /* Don't register the call */
                    }
                    else
                    {
                        /* New valid call */
                        DBG_PRINT_STRING("New outer call");
                        spstrElevator->aenuFloorCalls[u8FloorIndex] = CALL_EXTERNAL;

                        /* Set led pattern to internal */
                        LEDController_vidSetPattern(u8FloorIndex, LED_PATTERN_EXTERNAL_CALL);

                        if(spstrElevator->enuDirection == DIR_IDLE)
                        {
                            spstrElevator->u8DestinationFloor = u8FloorIndex;
                            if(spstrElevator->u8CurrentFloor < u8FloorIndex)
                            {
                                spstrElevator->enuDirection = DIR_UP;
                                spstrElevator->enuLastTravelDir = DIR_UP;
                            }
                            else
                            {
                                spstrElevator->enuDirection = DIR_DOWN;
                                spstrElevator->enuLastTravelDir = DIR_DOWN;
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
 */
void CallHandler_vidProcessCallQueue(void)
{
	uint8_t i;
    boolean bDestFound = FALSE;
    if(spstrElevator->enuLastTravelDir == DIR_UP)
    {
        DBG_PRINT_STRING("Processing call queue: last DIR_UP");

        /* 1. Scan floors above currentFloor for nearest CALL_INTERNAL → candidate_up_int */
        for(i=spstrElevator->u8CurrentFloor+1U;i<spstrElevator->u8FloorCount;i++)
        {
            /* if candidate_up_int exists → Dest = candidate_up_int, Dir = DIR_UP */
            if(spstrElevator->aenuFloorCalls[i] == CALL_INTERNAL)
            {
                spstrElevator->u8DestinationFloor = i;
                spstrElevator->enuDirection = DIR_UP;
                spstrElevator->enuLastTravelDir = DIR_UP;
                bDestFound = TRUE;
                break;
            }
        }
        /* 2. Scan floors above currentFloor for nearest CALL_ANY     → candidate_up_any */
        if(! bDestFound)
        {
            for(i=spstrElevator->u8CurrentFloor+1U;i<spstrElevator->u8FloorCount;i++)
            {
                /* elif candidate_up_any exists → Dest = candidate_up_any, Dir = DIR_UP */
                if(spstrElevator->aenuFloorCalls[i] != CALL_NONE)
                {
                    spstrElevator->u8DestinationFloor = i;
                    spstrElevator->enuDirection = DIR_UP;
                    spstrElevator->enuLastTravelDir = DIR_UP;
                    bDestFound = TRUE;
                    break;
                }
            }
        }
        /* 3. Scan floors below currentFloor for nearest CALL_ANY     → candidate_dn_any */
        if(! bDestFound)
        {
            if(spstrElevator->u8CurrentFloor > 0U)
            {
                for(i=spstrElevator->u8CurrentFloor-1U;i>0U;i--)
                {
                    /* elif candidate_dn_any exists → Dest = candidate_dn_any, Dir = DIR_DOWN */
                    if(spstrElevator->aenuFloorCalls[i] != CALL_NONE)
                    {
                        spstrElevator->u8DestinationFloor = i;
                        spstrElevator->enuDirection = DIR_DOWN;
                        spstrElevator->enuLastTravelDir = DIR_DOWN;
                        bDestFound = TRUE;
                        break;
                    }
                }
                /* loop stops at i=1, floor 0 must be checked explicitly */
                if(!bDestFound && spstrElevator->aenuFloorCalls[0] != CALL_NONE)
                {
                    spstrElevator->u8DestinationFloor = 0U;
                    spstrElevator->enuDirection = DIR_DOWN;
                    spstrElevator->enuLastTravelDir = DIR_DOWN;
                    bDestFound = TRUE;
                }
            }
            /* else: already at floor 0, no floors below → bDestFound stays FALSE → DIR_IDLE */
        }
        /* else → Dir = DIR_IDLE */
        if(! bDestFound)
        {
            spstrElevator->enuDirection = DIR_IDLE;
        }
    }
    else
    {
        DBG_PRINT_STRING("Processing call queue: last DIR_DN");
        
        bDestFound = FALSE;
        /* 1. Scan floors below currentFloor for nearest CALL_INTERNAL → candidate_dn_int */
        
        if(spstrElevator->u8CurrentFloor > 0U)
        {
            for(i=spstrElevator->u8CurrentFloor-1U;i>0U;i--)
            {
                /* if candidate_dn_int exists → Dest = candidate_dn_int, Dir = DIR_DOWN */
                if(spstrElevator->aenuFloorCalls[i] == CALL_INTERNAL)
                {
                    spstrElevator->u8DestinationFloor = i;
                    spstrElevator->enuDirection = DIR_DOWN;
                    spstrElevator->enuLastTravelDir = DIR_DOWN;
                    bDestFound = TRUE;
                    break;
                }
            }
            /* loop stops at i=1, floor 0 must be checked explicitly */
            if(!bDestFound && spstrElevator->aenuFloorCalls[0] == CALL_INTERNAL)
            {
                spstrElevator->u8DestinationFloor = 0U;
                spstrElevator->enuDirection = DIR_DOWN;
                spstrElevator->enuLastTravelDir = DIR_DOWN;
                bDestFound = TRUE;
            }
        }
        /* 2. Scan floors below currentFloor for nearest CALL_ANY      → candidate_dn_any */
        if(! bDestFound)
        {
            if(spstrElevator->u8CurrentFloor > 0U)
            {
                for(i=spstrElevator->u8CurrentFloor-1U;i>0U;i--)
                {
                    /* elif candidate_dn_any exists → Dest = candidate_dn_any, Dir = DIR_DOWN */
                    if(spstrElevator->aenuFloorCalls[i] != CALL_NONE)
                    {
                        spstrElevator->u8DestinationFloor = i;
                        spstrElevator->enuDirection = DIR_DOWN;
                        spstrElevator->enuLastTravelDir = DIR_DOWN;
                        bDestFound = TRUE;
                        break;
                    }
                }
                /* loop stops at i=1, floor 0 must be checked explicitly */
                if(!bDestFound && spstrElevator->aenuFloorCalls[0] != CALL_NONE)
                {
                    spstrElevator->u8DestinationFloor = 0U;
                    spstrElevator->enuDirection = DIR_DOWN;
                    spstrElevator->enuLastTravelDir = DIR_DOWN;
                    bDestFound = TRUE;
                }
            }
        }
        /* 3. Scan floors above currentFloor for nearest CALL_ANY      → candidate_up_any */
        if(! bDestFound)
        {
            for(i=spstrElevator->u8CurrentFloor+1;i<spstrElevator->u8FloorCount;i++)
            {
                /* elif candidate_up_any exists → Dest = candidate_up_any, Dir = DIR_UP */
                if(spstrElevator->aenuFloorCalls[i] != CALL_NONE)
                {
                    spstrElevator->u8DestinationFloor = i;
                    spstrElevator->enuDirection = DIR_UP;
                    spstrElevator->enuLastTravelDir = DIR_UP;
                    bDestFound = TRUE;
                    break;
                }
            }
        }
        /* else → Dir = DIR_IDLE */
        if(! bDestFound)
        {
            spstrElevator->enuDirection = DIR_IDLE;
        }
    }
}
/* ************************************************************************ */


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
