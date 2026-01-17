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

#define ElevatorController_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include <avr/interrupt.h>
#include "Std_Types.h"
#include <util/delay.h>

/* other components of the project **************************************** */

#include "SystemConfig.h"
#include "Timer.h"
#include "RelayManager.h"
#include "LEDController.h"
#include "CallHandler.h"
#include "SegmentDriver.h"
#include "lcd_config.h"
#include "ButtonDriver.h"
#include "SensorManager.h"
#include "MotionController.h"
#include "EepD.h"
#include "Menu.h"


/* own header inclusions ************************************************** */

/* access to its own exports */
#include "ElevatorController.h"

/* access to the modul's local configuration */
#include "ElevatorController_lcfg.h"


/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */


static OperatingMode_t enuCurrentMode = MODE_INIT;

static Elevator_t strElevator = {0}; // Main elevator state

uint8_t u8AccessFlag = 0;
uint16_t u16EnteredPass = 0, u16StoredPass = 0, u16MasterPass = 0;


/**
 * @brief Updates the menu items based on the current state
 * 
 */
static void updateMenuItems(void);

/**
 * @brief Construct a new eeprom loadvalues object
 * 
 */
static void EEPROM_LoadValues(void);

/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */

/*
 * @brief Initialize the elevator controller.
 * @description This function sets up the elevator controller
 * by initializing the necessary hardware components and configurations.
*/
void ElevatorController_Init(void)
{
    elevator_hal_vidInit();
    
    elevator_hal_vidTimer_start(); // Start the timer

    /* Update values from EEPROM */
    EEPROM_LoadValues();

    updateMenuItems();

    Menu_Update();
}

// HAL initialization
void elevator_hal_vidInit(void)
{
    /* Initialize LCD */
    LCD_Init();
    
    /* Initialize 7-Segment Display */
    SegmentDriver_vidInit();

    /* Initialize Relay Manager */
    RelayManager_vidInit();

    /* Initialize LED Controller */
    LEDController_vidInit();

    /* Initialize Call Handler */
    CallHandler_vidInit();

    /* Initialize sensors */
    (void) SensorManager_stdInit();

    /* Initialize buttons */
    ButtonDriver_vidInit();

    /* Initialize motion controller */
    MotionController_vidInit();    
    
    /* Initialize Menu module */
    Menu_Init();

    /* Initialize system timer */
    Timer_cfg_t strTimer;
    strTimer.enuTimerCH = SYSTEM_TIMER_CHANNEL;
    strTimer.enumTimerIntMode = TIMER_INT_DISABLED;
    strTimer.enuTimerPre = TIMER_PRESCALER_1024; // Example prescaler value
    strTimer.CBK_Ptr = NULL; // No callback function for now
    Timer_Init(&strTimer);

    /* Enable global interrupts */
    sei();
}

void ElevatorController_vidSplashScreen(void)
{
    uint8_t u8Index;
    // Display splash screen on LCD
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString("Elevator System");

    /* Blink all LEDs */
    for (u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index++)
    {
        LEDController_vidSetState(u8Index, LED_STATE_ON);
    }
    _delay_ms(500); // Wait for 500 milliseconds

    /* Blink every other LED */
    for (u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index++)
    {
        LEDController_vidSetState(u8Index, LED_STATE_OFF);
    }
    for (u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index += 2)
    {
        LEDController_vidSetState(u8Index, LED_STATE_ON);
    }
    _delay_ms(500); // Wait for 500 milliseconds
    
    /* Blink every other LED */
    for (u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index++)
    {
        LEDController_vidSetState(u8Index, LED_STATE_OFF);
    }
    for (u8Index = 1; u8Index < cu8MAX_FLOORS; u8Index += 2)
    {
        LEDController_vidSetState(u8Index, LED_STATE_ON);
    }
    _delay_ms(500); // Wait for 500 milliseconds
    
    /* Turn LEDs off */
    for (u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index++)
    {
        LEDController_vidSetState(u8Index, LED_STATE_OFF);
    }

    /* Detecting fresh flash */
    if(strElevator.u8SlowTimer == 0xFF)
    {
        /* Fresh flash detected, reset defaults */
        LCD_SetCursor(0, 0);
        LCD_WriteString("Initializing...");
        vidResetDefaults();
        _delay_ms(2000); // Display for 2 seconds
        LCD_SetCursor(1, 0);
        LCD_WriteString("               ");
    }
    else
    {
        /* Not a fresh flash, proceed with normal operation */
    }

    LCD_SetCursor(1, 0);
    LCD_WriteString("Booting");
    _delay_ms(500); // Display for 500 milliseconds
    LCD_WriteChar('.');
    _delay_ms(500); // Display for 500 milliseconds
    LCD_WriteChar('.');
    _delay_ms(500); // Display for 500 milliseconds
    LCD_WriteChar('.');
    _delay_ms(500); // Display for 500 milliseconds
    LCD_Clear();
}

OperatingMode_t ElevatorController_enuGetMode(void)
{
    if(ButtonDriver_bIsPressed(BTN_PRG))
    {
        enuCurrentMode = MODE_PROGRAM;
    }
    else
    {
        enuCurrentMode = MODE_NORMAL;
    }

    return enuCurrentMode;
}

void ElevatorController_vidOperationLoop(void)
{    
    // Initialize elevator state
    strElevator.u8CurrentFloor = 0;
    strElevator.enuDirection = DIR_IDLE;
    strElevator.enuDoorState = DOOR_CLOSED;
    strElevator.enuOperatingMode = MODE_NORMAL;
    strElevator.bEmergencyStop = FALSE;
    
    while(1)
    {
        
    }
}

void ElevatorController_vidProgrammingLoop(void)
{
    /* Programming mode */
    uint8_t u8Tmp = 0;

    LCD_SetCursor(0, 0);
    LCD_WriteString("Insert pass:");

    /* Read stored user password */
    EEPROM_u8Read(cu8PASSWORD_H_EE_ADD, &u8Tmp);
    u16StoredPass = (u8Tmp << 8) & 0xF0;
    EEPROM_u8Read(cu8PASSWORD_L_EE_ADD, &u8Tmp);
    u16StoredPass |= u8Tmp;
    

    /* Read stored master password */
    EEPROM_u8Read(cu8MASTER_PASSWORD_H_EE_ADD, &u8Tmp);
    u16MasterPass = (u8Tmp << 8) & 0xF0;
    EEPROM_u8Read(cu8MASTER_PASSWORD_L_EE_ADD, &u8Tmp);
    u16MasterPass |= u8Tmp;

    if(u16StoredPass > 999)
        u16StoredPass = 0;
    else
    {
        /* Do nothing */
    }

    while(1)
    {
        if(!u8AccessFlag)
        {
            /* Access not granted yet */

            LCD_SetCursor(1, 5);
            LCD_send_int(u16EnteredPass, 3);
            
            /* get password entry from user */
            if(ButtonDriver_bIsPressed(BTN_UP))
            {
                if(u16EnteredPass < 999)
                {
                    u16EnteredPass++;
                }
                else
                {
                    /* Do nothing */
                }
            }
            
            if(ButtonDriver_bIsPressed(BTN_DN))
            {
                if(u16EnteredPass > 0)
                {
                    u16EnteredPass--;
                }
                else
                {
                    /* Do nothing */
                }
            }
            
            
            if(ButtonDriver_bIsPressed(BTN_OK))
            {
                /* Check entered password */
                if((u16EnteredPass == u16StoredPass) || (u16EnteredPass == u16MasterPass))
                {
                    u8AccessFlag = TRUE;
                    LCD_SetCursor(0, 0);
                    LCD_WriteString("Access Granted");
                    _delay_ms(1000); // Display for 1 second
                    LCD_Clear();
                    Menu_Update();
                }
                else
                {
                    u8AccessFlag = FALSE;
                    LCD_SetCursor(0, 0);
                    LCD_WriteString("Access Denied");
                    _delay_ms(1000); // Display for 1 second
                    LCD_SetCursor(1, 0);
                    LCD_WriteString("             ");
                }
            }
        }
        else
        {
            /* Display config menu */
            Menu_Task();
        }
    }
}

/* Services ********************************************************* */

/**
 * @brief Retrieves the current floor of the elevator.
 * 
 */
uint8_t ElevatorController_u8GetCurrentFloor(void)
{
    return 0;
}

/**
 * @brief Retrieves the selector pole count of the elevator.
 * 
 * @return uint8_t 
 */
uint8_t ElevatorController_u8GetSelectorCnt(void)
{
    return 0;
}


/* ************************************************************************ */

// Motor control
void elevator_hal_vidMotor_up(void)
{

}

void elevator_hal_vidMotor_down(void)
{

}

void elevator_hal_vidMotor_stop(void)
{

}

// Door control
void elevator_hal_vidDoor_open(void)
{
    RelayManager_vidActivateRelay(RELAY_DOOR);
}

void elevator_hal_vidDoor_close(void)
{
    RelayManager_vidDeActivateRelay(RELAY_DOOR);
}

// Display functions
void elevator_hal_vidDisplay_floor(uint8_t u8Floor)
{

}

void elevator_hal_vidDisplay_status(const uint8_t* cpu8Status)
{

}

// Timer functions
void elevator_hal_vidTimer_start()
{
    Timer_Start(SYSTEM_TIMER_CHANNEL);
}

void elevator_hal_vidTimer_stop(void)
{
    Timer_Stop(SYSTEM_TIMER_CHANNEL);
}

boolean elevator_hal_bTimer_elapsed(void)
{
    return FALSE;
}

uint16_t elevator_hal_u16Get_time_ms(void)
{
    return Timer_GetValue(SYSTEM_TIMER_CHANNEL);
}

void vidResetDefaults(void)
{
	/* RESET_DEFAULT_VALUES */
	(void) EEPROM_u8Write(cu8SLOW_TIMER_EE_ADD, cu8SLOW_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8FAST_TIMER_EE_ADD, cu8FAST_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CAM_TIMER_EE_ADD, cu8CAM_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8STOP_TIMER_EE_ADD, cu8STOP_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8LIGHT_TIMER_EE_ADD, cu8LIGHT_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8DOOR_NUMBER_EE_ADD, cu8DOOR_NUMBER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8COLLECTION_DIR_EE_ADD, cu8COLLECTION_DIR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8MNT_SPEED_EE_ADD, cu8MNT_SPEED_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8DOOR_OPTIONS_EE_ADD, cu8DOOR_OPTIONS_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8PARKING_FLOOR_EE_ADD, cu8PARKING_FLOOR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CAM_FAIL_CNT_EE_ADD, cu8CAM_FAIL_CNT_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CABIN_PW_EE_ADD, cu8CABIN_PW_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8PHS_SEQ_EE_ADD, cu8PHS_SEQ_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8PASSWORD_L_EE_ADD, cu8PASSWORD_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8PASSWORD_H_EE_ADD, cu8PASSWORD_H_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CURRENT_FLOOR_EE_ADD, cu8CURRENT_FLOOR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CURRENT_TRAVEL_L_EE_ADD, cu8CURRENT_TRAVEL_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8CURRENT_TRAVEL_H_EE_ADD, cu8CURRENT_TRAVEL_H_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8STORED_TRAVEL_EE_ADD, cu8STORED_TRAVEL_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8STORED_TRAVEL_USAGE_EE_ADD, cu8STORED_TRAVEL_USAGE_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8MASTER_PASSWORD_L_EE_ADD, cu8MASTER_PASSWORD_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Write(cu8MASTER_PASSWORD_H_EE_ADD, cu8MASTER_PASSWORD_H_DEF_VALUE);
    _delay_ms(20);
}

/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */


/**
 * @brief Updates the menu items based on the current state
 * 
 */
static void updateMenuItems(void)
{
    Menu_SetItemData(SLOW_TIMER_MID, "Slow:", strElevator.u8SlowTimer, cu8SLOW_TIMER_MIN_VALUE, cu8SLOW_TIMER_MAX_VALUE, cu8SLOW_TIMER_EE_ADD);
    Menu_SetItemData(FAST_TIMER_MID, "Fast:", strElevator.u8FastTimer, cu8FAST_TIMER_MIN_VALUE, cu8FAST_TIMER_MAX_VALUE, cu8FAST_TIMER_EE_ADD);
    Menu_SetItemData(CAM_TIMER__MID, "CAM:", strElevator.u8CamTimer, cu8CAM_TIMER_MIN_VALUE, cu8CAM_TIMER_MAX_VALUE, cu8CAM_TIMER_EE_ADD);
    Menu_SetItemData(STOP_TIMER_MID, "Stop:", strElevator.u8StopTimer, cu8STOP_TIMER_MIN_VALUE, cu8STOP_TIMER_MAX_VALUE, cu8STOP_TIMER_EE_ADD);
    Menu_SetItemData(LIGHT_TIMER_MID, "Light:", strElevator.u8LightTimer, cu8LIGHT_TIMER_MIN_VALUE, cu8LIGHT_TIMER_MAX_VALUE, cu8LIGHT_TIMER_EE_ADD);
    Menu_SetItemData(DOOR_NUMBER_MID, "Floor Cnt:", strElevator.u8FloorCount, cu8DOOR_NUMBER_MIN_VALUE, cu8DOOR_NUMBER_MAX_VALUE, cu8DOOR_NUMBER_EE_ADD);
    Menu_SetItemData(COLLECTION_DIR_MID, "Col dir:", strElevator.u8CollectionDir, cu8COLLECTION_DIR_MIN_VALUE, cu8COLLECTION_DIR_MAX_VALUE, cu8COLLECTION_DIR_EE_ADD);
    Menu_SetItemData(MNT_SPEED_MID, "Mnt speed:", strElevator.u8MntSpeed, cu8MNT_SPEED_MIN_VALUE, cu8MNT_SPEED_MAX_VALUE, cu8MNT_SPEED_EE_ADD);
    Menu_SetItemData(DOOR_OPTIONS_MID, "Door type:", (uint8_t)strElevator.enuDoorType, cu8DOOR_OPTIONS_MIN_VALUE, cu8DOOR_OPTIONS_MAX_VALUE, cu8DOOR_OPTIONS_EE_ADD);
    Menu_SetItemData(PARKING_FLOOR_MID, "Park floor:", strElevator.u8ParkFloor, cu8PARKING_FLOOR_MIN_VALUE, cu8PARKING_FLOOR_MAX_VALUE, cu8PARKING_FLOOR_EE_ADD);
    Menu_SetItemData(CAM_FAIL_CNT_MID, "CAM Fail Cnt:", strElevator.u8CamFailCnt, cu8CAM_FAIL_CNT_MIN_VALUE, cu8CAM_FAIL_CNT_MAX_VALUE, cu8CAM_FAIL_CNT_EE_ADD);
    Menu_SetItemData(CABIN_PW_MID, "Cabin Pwd:", strElevator.u8CabinPwd, cu8CABIN_PW_MIN_VALUE, cu8CABIN_PW_MAX_VALUE, cu8CABIN_PW_EE_ADD);
    Menu_SetItemData(PHS_SEQ_MID, "Phs Seq:", strElevator.u8PhsSeq, cu8PHS_SEQ_MIN_VALUE, cu8PHS_SEQ_MAX_VALUE, cu8PHS_SEQ_EE_ADD);
}

/**
 * @brief Construct a new eeprom loadvalues object
 * 
 */
static void EEPROM_LoadValues(void)
{
    uint8_t u8ReadDelay = 5;
    (void) EEPROM_u8Read(cu8CURRENT_FLOOR_EE_ADD, &strElevator.u8CurrentFloor);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8SLOW_TIMER_EE_ADD, &strElevator.u8SlowTimer);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8FAST_TIMER_EE_ADD, &strElevator.u8FastTimer);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8CAM_TIMER_EE_ADD, &strElevator.u8CamTimer);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8STOP_TIMER_EE_ADD, &strElevator.u8StopTimer);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8LIGHT_TIMER_EE_ADD, &strElevator.u8LightTimer);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8DOOR_NUMBER_EE_ADD, &strElevator.u8FloorCount);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8COLLECTION_DIR_EE_ADD, &strElevator.u8CollectionDir);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8MNT_SPEED_EE_ADD, &strElevator.u8MntSpeed);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8DOOR_OPTIONS_EE_ADD, (uint8_t *)&strElevator.enuDoorType);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8PARKING_FLOOR_EE_ADD, &strElevator.u8ParkFloor);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8CAM_FAIL_CNT_EE_ADD, &strElevator.u8CamFailCnt);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8CABIN_PW_EE_ADD, &strElevator.u8CabinPwd);
    _delay_ms(u8ReadDelay);
    (void) EEPROM_u8Read(cu8PHS_SEQ_EE_ADD, &strElevator.u8PhsSeq);
    _delay_ms(u8ReadDelay);
}

/* ************************************************************************ */


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
