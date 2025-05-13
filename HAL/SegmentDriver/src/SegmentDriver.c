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

#include <util/delay.h>
#define SegmentDriver_C

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "Std_Types.h"

/* system and library inclusions ****************************************** */

#include <util/delay.h>

/* other components of the project **************************************** */

#include "dio_types.h"
#include "dio.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "SegmentDriver.h"

/* access to the modul's local configuration */
#include "SegmentDriver_lcfg.h"

const uint8_t au8SegmentDigits[10] = {
    //HGFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111 // 9
};

/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/* Initialise the component. */

void SegmentDriver_vidInit (void)
{
    /* Initialize Segment DIOs */

    /* Index variable */
    uint8_t u8Index = 0U;
    
    /* Dio config struct */
    Dio_Cfg_t strSegmentDio = {0};
    strSegmentDio.enuPinDir = DIR_OUTPUT;

    for(u8Index = 0U; u8Index < cu8SEGMENT_COUNT; u8Index++)
    {
        /* Assign Segment from config table to Dio config struct */
        strSegmentDio.enuGPIO = strSegmentConfig[u8Index].enuSegmentPort;
        strSegmentDio.u8Pin = strSegmentConfig[u8Index].u8SegmentPin;

        /* Init Segment pin */
        DIO_Init(&strSegmentDio);
    }

    /* Init Segment latch pin */
    strSegmentDio.enuGPIO = cenuSegment_LATCH_Port;
    strSegmentDio.u8Pin = cu8Segment_LATCH_Pin;
    DIO_Init(&strSegmentDio);

    /* Turn off Segment display */
    DIO_WritePort(cenuSegment_A_Port, 0x00U);

    /* Send a pulse to shift register */
    
    /* Set Segment latch pin HIGH */
    DIO_WritePin(cenuSegment_LATCH_Port, cu8Segment_LATCH_Pin, STATE_HIGH);

    _delay_ms(1);

    /* Set Segment latch pin LOW */
    DIO_WritePin(cenuSegment_LATCH_Port, cu8Segment_LATCH_Pin, STATE_LOW);
}

/* ************************************************************************ */

/* Write on 7 segment display */
void SegmentDriver_vidWrite (uint8_t u8Value)
{
    
    /* Write value to segment pins */
    DIO_WritePort(strSegmentConfig[0].enuSegmentPort, au8SegmentDigits[u8Value%10]);

    /* Check if value is less than 10 */
    if (u8Value < 10U)
    {
        /* Set Segment H pin LOW */
        DIO_WritePin(cenuSegment_H_Port, cu8Segment_H_Pin, STATE_LOW);
    }
    else
    {
        /* Set Segment H pin HIGH */
        DIO_WritePin(cenuSegment_H_Port, cu8Segment_H_Pin, STATE_HIGH);
    }

    /* Send a pulse to shift register */
    
    /* Set Segment latch pin HIGH */
    DIO_WritePin(cenuSegment_LATCH_Port, cu8Segment_LATCH_Pin, STATE_HIGH);

    _delay_ms(1);

    /* Set Segment latch pin LOW */
    DIO_WritePin(cenuSegment_LATCH_Port, cu8Segment_LATCH_Pin, STATE_LOW);
}

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