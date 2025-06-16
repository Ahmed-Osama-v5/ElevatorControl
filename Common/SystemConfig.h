#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

/* Number of floors */
#define cu8MAX_FLOORS         ((uint8_t) 16U)


/* Pin definition for Latch */
#define SEG_LATCH_GPIO          ((GPIO_t) GPIOD)
#define SEG_LATCH_PIN           ((uint8_t) 2U)

#define CALL_0_LATCH_GPIO       ((GPIO_t) GPIOB)
#define CALL_0_LATCH_PIN        ((uint8_t) 2U)

#define CALL_1_LATCH_GPIO       ((GPIO_t) GPIOB)
#define CALL_1_LATCH_PIN        ((uint8_t) 3U)


#define CALL_0_OUT_GPIO         ((GPIO_t) GPIOD)
#define CALL_0_OUT_PIN          ((uint8_t) 4U)


#define CALL_1_OUT_GPIO         ((GPIO_t) GPIOD)
#define CALL_1_OUT_PIN          ((uint8_t) 3U)

/* Sensors definition */

#define SEN_0_INPUT_GPIO        ((GPIO_t) GPIOD)
#define SEN_0_INPUT_PIN         ((uint8_t) 7U)

#define SEN_1_INPUT_GPIO        ((GPIO_t) GPIOB)
#define SEN_1_INPUT_PIN         ((uint8_t) 7U)

#define SEL_1_GPIO              ((GPIO_t) GPIOB)
#define SEL_1_PIN               ((uint8_t) 6U)

#define SEL_2_GPIO              ((GPIO_t) GPIOB)
#define SEL_2_PIN               ((uint8_t) 5U)

#define SEL_3_GPIO              ((GPIO_t) GPIOB)
#define SEL_3_PIN               ((uint8_t) 4U)

#define SYSTEM_TIMER_CHANNEL    ((TimerChan_t) TIMER_CH1)


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