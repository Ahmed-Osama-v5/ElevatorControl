#ifndef ELEVATOR_CONTROLLER_LCFG_H
#define ELEVATOR_CONTROLLER_LCFG_H

#if ! ((defined ElevatorController_c) || (defined ElevatorController_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

typedef enum
{
    UP_COLLECTIVE = 0,
    DOWN_COLLECTIVE
} CollectionDir_t;

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

#endif // ELEVATOR_CONTROLLER_LCFG_H
