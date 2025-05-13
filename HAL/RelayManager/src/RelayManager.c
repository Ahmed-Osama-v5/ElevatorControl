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

#define RelayManager_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "Std_Types.h"

/* system and library inclusions ****************************************** */

// Remark:
// include header files of the system (e.g. operating system APIs or
// C standard libraries. In most cases, this section should be empty and
// completely removed, because we will not have a OS and we must not use
// C libraries.

/* other components of the project **************************************** */

#include "dio_types.h"
#include "dio.h"

/* other modules of the component ***************************************** */

// REMARK:
// this section is aplicable only, if this component is split into modules.
// Remove this swection otherwise completely.

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "RelayManager.h"

/* access to the modul's local configuration */
#include "RelayManager_lcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/* Initialise the component. */

void RelayManager_vidInit (void)
{
    /* Index variable */
    uint8_t u8Index = 0U;

    /* Dio config struct */
    Dio_Cfg_t strRelayDio;
    strRelayDio.enuPinDir = DIR_OUTPUT;

    /* Initialize relay pins */
    for(u8Index = 0U; u8Index < cu8RELAY_COUNT; u8Index++)
    {
        /* Assign relay from config table to Dio config struct */
        strRelayDio.enuGPIO = strRelayConfig[u8Index].enuRelayPort;
        strRelayDio.u8Pin = strRelayConfig[u8Index].u8RelayPin;

        /* Init relay pin */
        DIO_Init(&strRelayDio);

        /* De-energize relay coil */
        DIO_WritePin(strRelayDio.enuGPIO, strRelayDio.u8Pin, STATE_LOW);
    }
}

/* Activate relay. */
void RelayManager_vidActivateRelay (Relay_t enuRelay)
{
    switch (enuRelay)
    {
        case RELAY_DOOR:
            DIO_WritePin(strRelayConfig[RELAY_DOOR].enuRelayPort, strRelayConfig[RELAY_DOOR].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_HS:
            DIO_WritePin(strRelayConfig[RELAY_HS].enuRelayPort, strRelayConfig[RELAY_HS].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_LS:
            DIO_WritePin(strRelayConfig[RELAY_LS].enuRelayPort, strRelayConfig[RELAY_LS].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_UP:
            DIO_WritePin(strRelayConfig[RELAY_UP].enuRelayPort, strRelayConfig[RELAY_UP].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_DN:
            DIO_WritePin(strRelayConfig[RELAY_DN].enuRelayPort, strRelayConfig[RELAY_DN].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_CAM:
            DIO_WritePin(strRelayConfig[RELAY_CAM].enuRelayPort, strRelayConfig[RELAY_CAM].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_LIGHT:
            DIO_WritePin(strRelayConfig[RELAY_LIGHT].enuRelayPort, strRelayConfig[RELAY_LIGHT].u8RelayPin, STATE_HIGH);
            break;
        case RELAY_MNT:
            DIO_WritePin(strRelayConfig[RELAY_MNT].enuRelayPort, strRelayConfig[RELAY_MNT].u8RelayPin, STATE_HIGH);
            break;
        default:
            /* Do nothing */
            break;
    }
}

/* De-activate relay. */
void RelayManager_vidDeActivateRelay (Relay_t enuRelay)
{
    /* Index variable */
    uint8_t u8Index = 0U;

    switch (enuRelay)
    {
        case RELAY_DOOR:
            DIO_WritePin(strRelayConfig[RELAY_DOOR].enuRelayPort, strRelayConfig[RELAY_DOOR].u8RelayPin, STATE_LOW);
            break;
        case RELAY_HS:
            DIO_WritePin(strRelayConfig[RELAY_HS].enuRelayPort, strRelayConfig[RELAY_HS].u8RelayPin, STATE_LOW);
            break;
        case RELAY_LS:
            DIO_WritePin(strRelayConfig[RELAY_LS].enuRelayPort, strRelayConfig[RELAY_LS].u8RelayPin, STATE_LOW);
            break;
        case RELAY_UP:
            DIO_WritePin(strRelayConfig[RELAY_UP].enuRelayPort, strRelayConfig[RELAY_UP].u8RelayPin, STATE_LOW);
            break;
        case RELAY_DN:
            DIO_WritePin(strRelayConfig[RELAY_DN].enuRelayPort, strRelayConfig[RELAY_DN].u8RelayPin, STATE_LOW);
            break;
        case RELAY_CAM:
            DIO_WritePin(strRelayConfig[RELAY_CAM].enuRelayPort, strRelayConfig[RELAY_CAM].u8RelayPin, STATE_LOW);
            break;
        case RELAY_LIGHT:
            DIO_WritePin(strRelayConfig[RELAY_LIGHT].enuRelayPort, strRelayConfig[RELAY_LIGHT].u8RelayPin, STATE_LOW);
            break;
        case RELAY_MNT:
            DIO_WritePin(strRelayConfig[RELAY_MNT].enuRelayPort, strRelayConfig[RELAY_MNT].u8RelayPin, STATE_LOW);
            break;
        case RELAY_ALL:
            for(u8Index = 0U; u8Index < cu8RELAY_COUNT; u8Index++)
            {
                /* De-energize relay coil */
                DIO_WritePin(strRelayConfig[RELAY_MNT].enuRelayPort, strRelayConfig[RELAY_MNT].u8RelayPin, STATE_LOW);
            }
            break;
        default:
            /* Do nothing */
            break;
    }
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