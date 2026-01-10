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

typedef enum
{
    UP_COLLECTIVE = 0,
    DOWN_COLLECTIVE
} CollectionDir_t;

/* ************************************************************************ */
/* ************************************************************************ */
/*                            PERSISTENT DATA                               */
/* ************************************************************************ */
/* ************************************************************************ */

/* default values */
#define cu8SLOW_TIMER_DEF_VALUE             ((uint8_t) 7U)
#define cu8FAST_TIMER_DEF_VALUE             ((uint8_t) 18U)
#define cu8CAM_TIMER_DEF_VALUE              ((uint8_t) 3U)
#define cu8STOP_TIMER_DEF_VALUE             ((uint8_t) 2U)
#define cu8LIGHT_TIMER_DEF_VALUE            ((uint8_t) 10U)
#define cu8DOOR_NUMBER_DEF_VALUE            ((uint8_t) 16U)
#define cu8DISPLAY_DEF_VALUE                ((uint8_t) 0U)
#define cu8FLOOR_0_DEF_VALUE                ((uint8_t) 0U)
#define cu8FLOOR_1_DEF_VALUE                ((uint8_t) 1U)
#define cu8FLOOR_2_DEF_VALUE                ((uint8_t) 2U)
#define cu8FLOOR_3_DEF_VALUE                ((uint8_t) 3U)
#define cu8FLOOR_4_DEF_VALUE                ((uint8_t) 4U)
#define cu8FLOOR_5_DEF_VALUE                ((uint8_t) 5U)
#define cu8FLOOR_6_DEF_VALUE                ((uint8_t) 6U)
#define cu8FLOOR_7_DEF_VALUE                ((uint8_t) 7U)
#define cu8FLOOR_8_DEF_VALUE                ((uint8_t) 8U)
#define cu8FLOOR_9_DEF_VALUE                ((uint8_t) 9U)
#define cu8FLOOR_10_DEF_VALUE               ((uint8_t) 10U)
#define cu8FLOOR_11_DEF_VALUE               ((uint8_t) 11U)
#define cu8FLOOR_12_DEF_VALUE               ((uint8_t) 12U)
#define cu8FLOOR_13_DEF_VALUE               ((uint8_t) 13U)
#define cu8FLOOR_14_DEF_VALUE               ((uint8_t) 14U)
#define cu8FLOOR_15_DEF_VALUE               ((uint8_t) 15U)
#define cu8COLLECTION_DIR_DEF_VALUE         ((uint8_t) DOWN_COLLECTIVE)
#define cu8MNT_SPEED_DEF_VALUE              ((uint8_t) 0U)
#define cu8DOOR_OPTIONS_DEF_VALUE           ((uint8_t) 0U)
#define cu8PARKING_FLOOR_DEF_VALUE          ((uint8_t) 19U)
#define cu8CAM_FAIL_CNT_DEF_VALUE           ((uint8_t) 9U)
#define cu8CABIN_PW_DEF_VALUE               ((uint8_t) 0U)
#define cu8PHS_SEQ_DEF_VALUE                ((uint8_t) 0U)
#define cu8PASSWORD_L_DEF_VALUE             ((uint8_t) 0U)
#define cu8PASSWORD_H_DEF_VALUE             ((uint8_t) 0U)
#define cu8CURRENT_FLOOR_DEF_VALUE          ((uint8_t) 0U)
#define cu8CURRENT_TRAVEL_L_DEF_VALUE       ((uint8_t) 0U)
#define cu8CURRENT_TRAVEL_H_DEF_VALUE       ((uint8_t) 0U)
#define cu8STORED_TRAVEL_DEF_VALUE          ((uint8_t) 10U)
#define cu8STORED_TRAVEL_USAGE_DEF_VALUE    ((uint8_t) 0U)
#define cu8MASTER_PASSWORD_L_DEF_VALUE      ((uint8_t) 158U)
#define cu8MASTER_PASSWORD_H_DEF_VALUE      ((uint8_t) 0U)


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

#endif // ELEVATOR_CONTROLLER_LCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
