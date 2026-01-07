
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

#define MotionController_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include "Std_Types.h"

/* other components of the project **************************************** */

#include "RelayManager.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "MotionController.h"

/* access to the modul's local configuration */
#include "MotionController_lcfg.h"

/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */


/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */
/**
 * @brief Initialize the Motion Controller.
 */
void MotionController_vidInit(void)
{    
    /* Initialize status */
    
}

/**
 * @brief Move the elevator in a specific direction.
 * @param enuState The motion state and speed (up, down or stop).
 */
void MotionController_vidMove(MotionState_t enuState)
{
    switch (enuState)
    {
        case MOTION_STATE_MOVING_UP_HS:
            RelayManager_vidActivateRelay(RELAY_HS);
            RelayManager_vidActivateRelay(RELAY_UP);
            break;
            
        case MOTION_STATE_MOVING_DOWN_HS:
            RelayManager_vidActivateRelay(RELAY_HS);
            RelayManager_vidActivateRelay(RELAY_DN);
            break;

        case MOTION_STATE_MOVING_UP_LS:
            RelayManager_vidActivateRelay(RELAY_LS);
            RelayManager_vidActivateRelay(RELAY_UP);
            break;
            
        case MOTION_STATE_MOVING_DOWN_LS:
            RelayManager_vidActivateRelay(RELAY_LS);
            RelayManager_vidActivateRelay(RELAY_DN);
            break;
            
        case MOTION_STATE_IDLE:
            RelayManager_vidDeActivateRelay(RELAY_HS);
            RelayManager_vidDeActivateRelay(RELAY_LS);
            RelayManager_vidDeActivateRelay(RELAY_UP);
            RelayManager_vidDeActivateRelay(RELAY_DN);
            break;
            
        case MOTION_STATE_STOPPING:
            RelayManager_vidDeActivateRelay(RELAY_HS);
            RelayManager_vidDeActivateRelay(RELAY_LS);
            RelayManager_vidDeActivateRelay(RELAY_UP);
            RelayManager_vidDeActivateRelay(RELAY_DN);
            break;
            
        default:
            /* Invalid state */
            break;
    }
}

/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
