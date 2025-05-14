#ifndef LED_CONTROLLER_GCFG_H
#define LED_CONTROLLER_GCFG_H


#include "SystemConfig.h"

// Latch control pins for the two 74HC573s
#define cenuLED_LATCH0_PORT         ((GPIO_t) GPIOB)
#define cu8LED_LATCH0_PIN           ((uint8_t) 2U)
#define cenuLED_LATCH1_PORT         ((GPIO_t) GPIOB)
#define cu8LED_LATCH1_PIN           ((uint8_t) 3U)

// Data port for LEDs (8 bits at a time)
#define cenuLED_DATA_PORT           ((GPIO_t) GPIOA)

/* LED Hardware Configuration */
typedef struct {
    uint8_t u8LatchIndex;   // 0 for first 8 LEDs, 1 for second 8 LEDs
    uint8_t u8BitPos;       // Bit position in the latch (0-7)
}LEDHWConfig_t;

/* LED Pattern Timing Configurations (in milliseconds) */
#define cu16LED_INTERNAL_CALL_ON_TIME   ((uint16_t) 60000U)
#define cu16LED_INTERNAL_CALL_OFF_TIME  ((uint16_t) 0U)
#define cu16LED_EXTERNAL_CALL_ON_TIME   ((uint16_t) 10000U)
#define cu16LED_EXTERNAL_CALL_OFF_TIME  ((uint16_t) 10000U)
#define cu16LED_MAINTENANCE_ON_TIME     ((uint16_t) 200U)
#define cu16LED_MAINTENANCE_OFF_TIME    ((uint16_t) 200U)
#define cu16LED_ERROR_ON_TIME           ((uint16_t) 100U)
#define cu16LED_ERROR_OFF_TIME          ((uint16_t) 100U)

/* LED Controller Task Period (in milliseconds) */
#define cu8LED_CONTROLLER_PERIOD_MS   ((uint8_t) 10U)

/* LED CallType */
typedef enum {
    LED_CALL_TYPE_INTERNAL,
    LED_CALL_TYPE_EXTERNAL
} LEDCallType_t;

/* LED States */
typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
    LED_STATE_BLINKING
} LEDState_t;

/* LED Patterns */
typedef enum {
    LED_PATTERN_NONE,
    LED_PATTERN_INTERNAL_CALL,
    LED_PATTERN_EXTERNAL_CALL,
    LED_PATTERN_MAINTENANCE,
    LED_PATTERN_ERROR
} LEDPattern_t;

/* LED Configuration Structure */
typedef struct {
    uint8_t u8LedId;
    LEDPattern_t enuPattern;
    uint16_t u16OnTime;
    uint16_t u16OffTime;
} LEDConfig_t;
typedef struct
{
    boolean bIsActive;
    LEDCallType_t enuLEDCallType;
    uint16_t u16BlinkTimer;
    boolean bLEDState;
}FloorLED_t;


/* External declaration of LED configuration array */
extern const LEDHWConfig_t LED_HWConfig[cu8MAX_FLOORS];

#endif // LED_CONTROLLER_GCFG_H
