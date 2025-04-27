#ifndef LED_CONTROLLER_GCFG_H
#define LED_CONTROLLER_GCFG_H


#include "SystemConfig.h"

// Latch control pins for the two 74HC573s
#define cenuGPIOLED_LATCH0_PORT     GPIOB
#define cu8LED_LATCH0_PIN           ((uint8_t) 2U)
#define cenuGPIOLED_LATCH1_PORT     GPIOB
#define cu8LED_LATCH1_PIN           ((uint8_t) 3U)

// Data port for LEDs (8 bits at a time)
#define cenuGPIOLED_DATA_PORT       GPIOA

/* LED Hardware Configuration */
typedef struct {
    uint8_t u8LatchIndex;   // 0 for first 8 LEDs, 1 for second 8 LEDs
    uint8_t u8BitPos;       // Bit position in the latch (0-7)
}LEDHWConfig_t;


/* Maximum number of LEDs that can be controlled */
#define cu8LED_MAX_COUNT              cu8MAX_FLOORS

/* LED Pattern Timing Configurations (in milliseconds) */
#define cu16LED_INTERNAL_CALL_ON_TIME   ((uint16_t) 500U)
#define cu16LED_INTERNAL_CALL_OFF_TIME  ((uint16_t) 500U)
#define cu16LED_EXTERNAL_CALL_ON_TIME   ((uint16_t) 300U)
#define cu16LED_EXTERNAL_CALL_OFF_TIME  ((uint16_t) 700U)
#define cu16LED_MAINTENANCE_ON_TIME     ((uint16_t) 200U)
#define cu16LED_MAINTENANCE_OFF_TIME    ((uint16_t) 200U)
#define cu16LED_ERROR_ON_TIME           ((uint16_t) 100U)
#define cu16LED_ERROR_OFF_TIME          ((uint16_t) 100U)

/* LED Controller Task Period (in milliseconds) */
#define cu8LED_CONTROLLER_PERIOD_MS   ((uint8_t) 10U)

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
    CallType_t enuCallType;
    uint16_t u16BlinkTimer;
    boolean bLEDState;
}FloorLED_t;


/* External declaration of LED configuration array */
extern const LEDHWConfig_t LED_HWConfig[cu8LED_MAX_COUNT];

#endif // LED_CONTROLLER_GCFG_H
