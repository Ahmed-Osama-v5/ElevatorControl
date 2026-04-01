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
/* Protection against bad and multiple inclusions                           */
/* ************************************************************************ */

/**
 * @file ElevatorController_lcfg.h
 * @brief Local (private) configuration constants for the ElevatorController module.
 *
 * Contains EEPROM default values, minimum values, maximum values, and
 * internal type definitions used exclusively by ElevatorController.c
 * and ElevatorController_lcfg.c. Must not be included by any other module.
 *
 * @defgroup ElevatorController_lcfg ElevatorController Local Config
 * @ingroup App_ElevatorController
 * @{
 */

#ifndef ELEVATOR_CONTROLLER_LCFG_H
#define ELEVATOR_CONTROLLER_LCFG_H

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* other modules of the project ******************************************* */


#if ! ((defined ElevatorController_c) || (defined ElevatorController_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

/* ************************************************************************ */
/* ************************************************************************ */
/*                          CONFIGURATION DATA                              */
/* ************************************************************************ */
/* ************************************************************************ */

/** @brief Duration the door remains open before auto-close is triggered (ms). */
#define DOOR_OPEN_TIME_MS  3000

/**
 * @brief Direction used for the collective call algorithm.
 */
typedef enum
{
    UP_COLLECTIVE = 0, /**< Service calls in the upward direction first. */
    DOWN_COLLECTIVE    /**< Service calls in the downward direction first. */
} CollectionDir_t;

/* ************************************************************************ */
/* ************************************************************************ */
/*                            PERSISTENT DATA                               */
/* ************************************************************************ */
/* ************************************************************************ */

/** @name EEPROM Default Values
 *  Factory-default values written to EEPROM on first power-up.
 *  @{
 */
#define cu8SLOW_TIMER_DEF_VALUE             ((uint8_t) 7U)    /**< Default slow (decel) timer. */
#define cu8FAST_TIMER_DEF_VALUE             ((uint8_t) 18U)   /**< Default fast (high-speed) timer. */
#define cu8CAM_TIMER_DEF_VALUE              ((uint8_t) 3U)    /**< Default CAM pulse timer. */
#define cu8STOP_TIMER_DEF_VALUE             ((uint8_t) 2U)    /**< Default stop phase timer. */
#define cu8LIGHT_TIMER_DEF_VALUE            ((uint8_t) 10U)   /**< Default cabin light auto-off timer. */
#define cu8DOOR_NUMBER_DEF_VALUE            ((uint8_t) 16U)   /**< Default number of doors / floors. */
#define cu8DISPLAY_DEF_VALUE                ((uint8_t) 0U)    /**< Default display setting. */
#define cu8FLOOR_0_DEF_VALUE                ((uint8_t) 0U)    /**< Default floor 0 label. */
#define cu8FLOOR_1_DEF_VALUE                ((uint8_t) 1U)    /**< Default floor 1 label. */
#define cu8FLOOR_2_DEF_VALUE                ((uint8_t) 2U)    /**< Default floor 2 label. */
#define cu8FLOOR_3_DEF_VALUE                ((uint8_t) 3U)    /**< Default floor 3 label. */
#define cu8FLOOR_4_DEF_VALUE                ((uint8_t) 4U)    /**< Default floor 4 label. */
#define cu8FLOOR_5_DEF_VALUE                ((uint8_t) 5U)    /**< Default floor 5 label. */
#define cu8FLOOR_6_DEF_VALUE                ((uint8_t) 6U)    /**< Default floor 6 label. */
#define cu8FLOOR_7_DEF_VALUE                ((uint8_t) 7U)    /**< Default floor 7 label. */
#define cu8FLOOR_8_DEF_VALUE                ((uint8_t) 8U)    /**< Default floor 8 label. */
#define cu8FLOOR_9_DEF_VALUE                ((uint8_t) 9U)    /**< Default floor 9 label. */
#define cu8FLOOR_10_DEF_VALUE               ((uint8_t) 10U)   /**< Default floor 10 label. */
#define cu8FLOOR_11_DEF_VALUE               ((uint8_t) 11U)   /**< Default floor 11 label. */
#define cu8FLOOR_12_DEF_VALUE               ((uint8_t) 12U)   /**< Default floor 12 label. */
#define cu8FLOOR_13_DEF_VALUE               ((uint8_t) 13U)   /**< Default floor 13 label. */
#define cu8FLOOR_14_DEF_VALUE               ((uint8_t) 14U)   /**< Default floor 14 label. */
#define cu8FLOOR_15_DEF_VALUE               ((uint8_t) 15U)   /**< Default floor 15 label. */
#define cu8COLLECTION_DIR_DEF_VALUE         ((uint8_t) DOWN_COLLECTIVE) /**< Default collection direction. */
#define cu8MNT_SPEED_DEF_VALUE              ((uint8_t) 0U)    /**< Default maintenance speed index. */
#define cu8DOOR_OPTIONS_DEF_VALUE           ((uint8_t) 0U)    /**< Default door options byte. */
#define cu8PARKING_FLOOR_DEF_VALUE          ((uint8_t) 19U)   /**< Default parking floor (disabled = 19). */
#define cu8CAM_FAIL_CNT_DEF_VALUE           ((uint8_t) 9U)    /**< Default CAM failure count threshold. */
#define cu8CABIN_PW_DEF_VALUE               ((uint8_t) 0U)    /**< Default cabin password. */
#define cu8PHS_SEQ_DEF_VALUE                ((uint8_t) 0U)    /**< Default phase sequence byte. */
#define cu8PASSWORD_L_DEF_VALUE             ((uint8_t) 0U)    /**< Default access password low byte. */
#define cu8PASSWORD_H_DEF_VALUE             ((uint8_t) 0U)    /**< Default access password high byte. */
#define cu8CURRENT_FLOOR_DEF_VALUE          ((uint8_t) 0U)    /**< Default stored current floor. */
#define cu8CURRENT_TRAVEL_L_DEF_VALUE       ((uint8_t) 0U)    /**< Default travel counter low byte. */
#define cu8CURRENT_TRAVEL_H_DEF_VALUE       ((uint8_t) 0U)    /**< Default travel counter high byte. */
#define cu8STORED_TRAVEL_DEF_VALUE          ((uint8_t) 10U)   /**< Default stored travel distance. */
#define cu8STORED_TRAVEL_USAGE_DEF_VALUE    ((uint8_t) 0U)    /**< Default travel usage counter. */
#define cu8MASTER_PASSWORD_L_DEF_VALUE      ((uint8_t) 158U)  /**< Master password low byte. */
#define cu8MASTER_PASSWORD_H_DEF_VALUE      ((uint8_t) 0U)    /**< Master password high byte. */
/** @} */

/** @name EEPROM Minimum Values
 *  Lower bounds enforced when saving parameters via the programming menu.
 *  @{
 */


/* minimum values */
#define cu8SLOW_TIMER_MIN_VALUE             ((uint8_t) 1U)
#define cu8FAST_TIMER_MIN_VALUE             ((uint8_t) 1U)
#define cu8CAM_TIMER_MIN_VALUE              ((uint8_t) 1U)
#define cu8STOP_TIMER_MIN_VALUE             ((uint8_t) 1U)
#define cu8LIGHT_TIMER_MIN_VALUE            ((uint8_t) 1U)
#define cu8DOOR_NUMBER_MIN_VALUE            ((uint8_t) 1U)
#define cu8COLLECTION_DIR_MIN_VALUE         ((uint8_t) DOWN_COLLECTIVE)
#define cu8MNT_SPEED_MIN_VALUE              ((uint8_t) 0U)
#define cu8DOOR_OPTIONS_MIN_VALUE           ((uint8_t) 0U)
#define cu8PARKING_FLOOR_MIN_VALUE          ((uint8_t) 0U)
#define cu8CAM_FAIL_CNT_MIN_VALUE           ((uint8_t) 0U)
#define cu8CABIN_PW_MIN_VALUE               ((uint8_t) 0U)
#define cu8PHS_SEQ_MIN_VALUE                ((uint8_t) 0U)

/** @name EEPROM Maximum Values
 *  Upper bounds enforced when saving parameters via the programming menu.
 * 
 */

/* maximum values */
#define cu8SLOW_TIMER_MAX_VALUE             ((uint8_t) 20U)
#define cu8FAST_TIMER_MAX_VALUE             ((uint8_t) 30U)
#define cu8CAM_TIMER_MAX_VALUE              ((uint8_t) 20U)
#define cu8STOP_TIMER_MAX_VALUE             ((uint8_t) 10U)
#define cu8LIGHT_TIMER_MAX_VALUE            ((uint8_t) 20U)
#define cu8DOOR_NUMBER_MAX_VALUE            ((uint8_t) 16U)
#define cu8COLLECTION_DIR_MAX_VALUE         ((uint8_t) 4U)
#define cu8MNT_SPEED_MAX_VALUE              ((uint8_t) 1U)
#define cu8DOOR_OPTIONS_MAX_VALUE           ((uint8_t) 1U)
#define cu8PARKING_FLOOR_MAX_VALUE          (cu8DOOR_NUMBER_MAX_VALUE)
#define cu8CAM_FAIL_CNT_MAX_VALUE           ((uint8_t) 20U)
#define cu8CABIN_PW_MAX_VALUE               ((uint8_t) 1U)
#define cu8PHS_SEQ_MAX_VALUE                ((uint8_t) 1U)

/** @} */

#endif // ELEVATOR_CONTROLLER_LCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
