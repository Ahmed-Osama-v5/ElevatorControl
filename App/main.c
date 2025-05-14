/**
 * @file main.c
 * @brief Main application file for testAVR
 */
#ifndef F_CPU
 #define F_CPU	16000000UL
 #endif
#include <avr/io.h>
#include <util/delay.h>
#include "char_lcd.h"
#include "SystemConfig.h"
 #include "dio.h"
#include "dio_types.h"
#include "MotionController.h"
#include "SegmentDriver.h"

/* Local Function prototypes */
static void vidSystem_Init();
static PinState_t readSensor(Sensor_t sensor);

int main(void)
{
    /* Initialize system */
    vidSystem_Init();

    LCD_init();
    LCD_send_string("Elevator V1.1");

    uint8_t u8Floor = 0;

    while (1)
    {
        for(u8Floor = 0; u8Floor < cu8MAX_FLOORS; u8Floor++)
        {
            /* Display floor number */
            SegmentDriver_vidWrite(u8Floor);
            LCD_goto_xy(0, LINE_1);
            LCD_send_int(u8Floor, 2);
            _delay_ms(1000);
        }
        SegmentDriver_vidTurnOff();
        _delay_ms(1000);
    }

    return 0;
}


static void vidSystem_Init()
{
    /* Initialize motion controller */
    MotionController_stdInit();

    /* Initialize Segment driver */
    SegmentDriver_vidInit();

    /* Initialize DIOs */
    Dio_Cfg_t strDio = {0};

    /* CALL_0_LATCH pin */
    strDio.enuGPIO = CALL_0_LATCH_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = CALL_0_LATCH_PIN;
    DIO_Init(&strDio);

    /* CALL_1_LATCH pin */
    strDio.enuGPIO = CALL_1_LATCH_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = CALL_1_LATCH_PIN;
    DIO_Init(&strDio);
    

    /* CALL_0_OUT pin */
    strDio.enuGPIO = CALL_0_OUT_GPIO;
    strDio.enuPinDir = DIR_INPUT;
    strDio.u8Pin = CALL_0_OUT_PIN;
    DIO_Init(&strDio);
    

    /* CALL_1_OUT pin */
    strDio.enuGPIO = CALL_1_OUT_GPIO;
    strDio.enuPinDir = DIR_INPUT;
    strDio.u8Pin = CALL_1_OUT_PIN;
    DIO_Init(&strDio);
    
    DIO_WritePin(CALL_0_LATCH_GPIO, CALL_0_LATCH_PIN, STATE_LOW);
    DIO_WritePin(CALL_1_LATCH_GPIO, CALL_1_LATCH_PIN, STATE_LOW);


    /* Init sensor control pins */
    strDio.enuGPIO = SEN_0_INPUT_GPIO;
    strDio.enuPinDir = DIR_INPUT;
    strDio.u8Pin = SEN_0_INPUT_PIN;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = SEN_1_INPUT_GPIO;
    strDio.enuPinDir = DIR_INPUT;
    strDio.u8Pin = SEN_1_INPUT_PIN;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = SEL_1_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = SEL_1_PIN;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = SEL_2_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = SEL_2_PIN;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = SEL_3_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = SEL_3_PIN;
    DIO_Init(&strDio);
}

static PinState_t readSensor(Sensor_t sensor)
{
    PinState_t retVal = STATE_LOW;
    
    switch(sensor)
    {
        case FLOOR_M:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case SELECTOR:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case STOP_SEN:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case SHK:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case LOCK:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case UP_LIMIT:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case DN_LIMIT:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case MNT_SEN:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case MNT_UP:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case MNT_DN:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case OPEN_DOOR:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case OVER_LOAD:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case CLOSE_DOOR:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case FL:
			DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
			DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
			DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case PHASE_1:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case PHASE_2:
			DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
			DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
			DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        default:
            break;
    }
    return retVal;
}
