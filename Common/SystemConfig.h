#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/* Number of floors */
#define cu8MAX_FLOORS                   ((uint8_t) 16U)


/* Pin definition for Latch */
#define SEG_LATCH_GPIO                  ((GPIO_t) GPIOD)
#define SEG_LATCH_PIN                   ((uint8_t) 2U)

#define CALL_0_LATCH_GPIO               ((GPIO_t) GPIOB)
#define CALL_0_LATCH_PIN                ((uint8_t) 2U)

#define CALL_1_LATCH_GPIO               ((GPIO_t) GPIOB)
#define CALL_1_LATCH_PIN                ((uint8_t) 3U)


#define CALL_0_OUT_GPIO                 ((GPIO_t) GPIOD)
#define CALL_0_OUT_PIN                  ((uint8_t) 4U)

#define CALL_1_OUT_GPIO                 ((GPIO_t) GPIOD)
#define CALL_1_OUT_PIN                  ((uint8_t) 3U)

/* Sensors definition */

#define SEN_0_INPUT_GPIO                ((GPIO_t) GPIOD)
#define SEN_0_INPUT_PIN                 ((uint8_t) 7U)

#define SEN_1_INPUT_GPIO                ((GPIO_t) GPIOB)
#define SEN_1_INPUT_PIN                 ((uint8_t) 7U)

#define SEL_1_GPIO                      ((GPIO_t) GPIOB)
#define SEL_1_PIN                       ((uint8_t) 6U)

#define SEL_2_GPIO                      ((GPIO_t) GPIOB)
#define SEL_2_PIN                       ((uint8_t) 5U)

#define SEL_3_GPIO                      ((GPIO_t) GPIOB)
#define SEL_3_PIN                       ((uint8_t) 4U)

#define SYSTEM_TIMER_CHANNEL            ((TimerChan_t) TIMER_CH1)


/* EEPROM addresses */
#define cu8SLOW_TIMER_EE_ADD            ((uint8_t) 0x00)
#define cu8FAST_TIMER_EE_ADD            ((uint8_t) 0x01)
#define cu8CAM_TIMER_EE_ADD             ((uint8_t) 0x02)
#define cu8STOP_TIMER_EE_ADD            ((uint8_t) 0x03)
#define cu8LIGHT_TIMER_EE_ADD           ((uint8_t) 0x04)

#define cu8DOOR_NUMBER_EE_ADD           ((uint8_t) 0x05)

#define cu8DISPLAY_EE_ADD               ((uint8_t) 0x06)
#define cu8FLOOR_0_EE_ADD               ((uint8_t) 0x07)
#define cu8FLOOR_1_EE_ADD               ((uint8_t) 0x08)
#define cu8FLOOR_2_EE_ADD               ((uint8_t) 0x09)
#define cu8FLOOR_3_EE_ADD               ((uint8_t) 0x0A)
#define cu8FLOOR_4_EE_ADD               ((uint8_t) 0x0B)
#define cu8FLOOR_5_EE_ADD               ((uint8_t) 0x0C)
#define cu8FLOOR_6_EE_ADD               ((uint8_t) 0x0D)
#define cu8FLOOR_7_EE_ADD               ((uint8_t) 0x0E)
#define cu8FLOOR_8_EE_ADD               ((uint8_t) 0x0F)
#define cu8FLOOR_9_EE_ADD               ((uint8_t) 0x10)
#define cu8FLOOR_10_EE_ADD              ((uint8_t) 0x11)
#define cu8FLOOR_11_EE_ADD              ((uint8_t) 0x12)
#define cu8FLOOR_12_EE_ADD              ((uint8_t) 0x13)
#define cu8FLOOR_13_EE_ADD              ((uint8_t) 0x14)
#define cu8FLOOR_14_EE_ADD              ((uint8_t) 0x15)
#define cu8FLOOR_15_EE_ADD              ((uint8_t) 0x16)

#define cu8COLLECTION_DIR_EE_ADD        ((uint8_t) 0x17)
#define cu8MNT_SPEED_EE_ADD             ((uint8_t) 0x18)
#define cu8DOOR_OPTIONS_EE_ADD          ((uint8_t) 0x19)
#define cu8PARKING_FLOOR_EE_ADD         ((uint8_t) 0x1A)
#define cu8CAM_FAIL_CNT_EE_ADD          ((uint8_t) 0x1B)
#define cu8CABIN_PW_EE_ADD              ((uint8_t) 0x1C)
#define cu8PHS_SEQ_EE_ADD               ((uint8_t) 0x1D)

#define cu8PASSWORD_L_EE_ADD            ((uint8_t) 0x1E)
#define cu8PASSWORD_H_EE_ADD            ((uint8_t) 0x1F)

#define cu8CURRENT_FLOOR_EE_ADD         ((uint8_t) 0x20)

#define cu8CURRENT_TRAVEL_L_EE_ADD      ((uint8_t) 0x21)
#define cu8CURRENT_TRAVEL_H_EE_ADD      ((uint8_t) 0x22)
#define cu8STORED_TRAVEL_EE_ADD         ((uint8_t) 0x23)
#define cu8STORED_TRAVEL_USAGE_EE_ADD   ((uint8_t) 0x26)

#define cu8MASTER_PASSWORD_L_EE_ADD     ((uint8_t) 0x24)
#define cu8MASTER_PASSWORD_H_EE_ADD     ((uint8_t) 0x25)


typedef enum
{
    MNT_SEN,
    DN_LIMIT,
    UP_LIMIT,
    LOCK,
    SHK,
    STOP_SEN,
    SELECTOR,
    FLOOR_M,
    PHASE_2,
    PHASE_1,
    FULL_LOAD,
    CLOSE_DOOR,
    OVER_LOAD,
    OPEN_DOOR,
    MNT_DN,
    MNT_UP,
    NO_SENSOR
}Sensor_t;

#endif // SYSTEM_CONFIG_H