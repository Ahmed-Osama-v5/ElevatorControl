/**
 * @file main.c
 * @brief Main application file for testAVR
 */
#include "ElevatorController.h"
#include "SensorManager.h"
#include <stdint.h>
#ifndef F_CPU
 #define F_CPU	8000000UL
 #endif
#include <avr/io.h>
#include <util/delay.h>
#include "char_lcd.h"
#include "SystemConfig.h"
 #include "dio.h"
 #include "Timer.h"
#include "dio_types.h"
#include "MotionController.h"
#include "SegmentDriver.h"
#include "LEDController.h"
#include "ButtonDriver.h"

/* Local Function prototypes */
static void vidSystem_Init();
static PinState_t readSensor(Sensor_t enuSensor);
static Sensor_t getSensor(void);

int main(void)
{
    /* Initialize system */
    vidSystem_Init();

    while (1)
    {

    }

    return 0;
}


static void vidSystem_Init()
{
    /* Initialize motion controller */
    (void) MotionController_stdInit();

    /* Initialize Segment driver */
    SegmentDriver_vidInit();

    /* Initialize LEDController */
    LEDController_vidInit();

    /* Initialize SensorManager */
    (void) SensorManager_stdInit();

    Timer_cfg_t strTimer = {0};
    strTimer.enuTimerCH = TIMER_CH1;
    strTimer.enumTimerIntMode = TIMER_INT_DISABLED;
    strTimer.enuTimerPre = TIMER_PRESCALER_1024;
    strTimer.CBK_Ptr = NULL;
    Timer_Init(&strTimer);
    Timer_Start(TIMER_CH1, 0);

    /* Initialize DIOs */
    Dio_Cfg_t strDio = {0};    

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

    /* Initialize buttons */
    ButtonDriver_vidInit();

    /* Initialize LCD */
    LCD_init();

    if(ButtonDriver_bIsPressed(BTN_PRG) == True)
    {
        /* Enter programming mode */
        LCD_goto_xy(0, LINE_1);
        LCD_send_string("Programming Mode");
    }
    else
    {
        /* Initialize the system in normal mode */
    }
}

static PinState_t readSensor(Sensor_t enuSensor)
{
    PinState_t retVal = STATE_LOW;
    
    switch(enuSensor)
    {
        case FLOOR_M:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case SELECTOR:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case STOP_SEN:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case SHK:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_HIGH);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case LOCK:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case UP_LIMIT:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case DN_LIMIT:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
        case MNT_SEN:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_1_INPUT_GPIO, SEN_1_INPUT_PIN);
            break;
            /* **************************************************************** */
            /* **************************************************************** */
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
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
            DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case FULL_LOAD:
			DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_LOW);
			DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_HIGH);
			DIO_WritePin(SEL_3_GPIO, SEL_3_PIN, STATE_LOW);
            _delay_ms(1);
            retVal = DIO_ReadPin(SEN_0_INPUT_GPIO, SEN_0_INPUT_PIN);
            break;
        case PHASE_1:
            DIO_WritePin(SEL_1_GPIO, SEL_1_PIN, STATE_HIGH);
            DIO_WritePin(SEL_2_GPIO, SEL_2_PIN, STATE_LOW);
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

static Sensor_t getSensor(void)
{
    PinState_t enuSensorState = STATE_LOW;
    enuSensorState = readSensor(FLOOR_M);
    if(enuSensorState == STATE_HIGH)
    {
        return FLOOR_M;
    }
    enuSensorState = readSensor(STOP_SEN);
    if(enuSensorState == STATE_HIGH)
    {
        return STOP_SEN;
    }
    enuSensorState = readSensor(SHK);
    if(enuSensorState == STATE_HIGH)
    {
        return SHK;
    }
    enuSensorState = readSensor(LOCK);
    if(enuSensorState == STATE_HIGH)
    {
        return LOCK;
    }
    enuSensorState = readSensor(UP_LIMIT);
    if(enuSensorState == STATE_HIGH)
    {
        return UP_LIMIT;
    }
    enuSensorState = readSensor(DN_LIMIT);
    if(enuSensorState == STATE_HIGH)
    {
        return DN_LIMIT;
    }
    enuSensorState = readSensor(MNT_SEN);
    if(enuSensorState == STATE_HIGH)
    {
        return MNT_SEN;
    }
    return NO_SENSOR;
}