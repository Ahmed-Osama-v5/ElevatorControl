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

#define TEST_

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

#include "debug_log.h"   // ← only this, never uart.h directly

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "ElevatorController.h"

/* access to the modul's local configuration */
#include "ElevatorController_lcfg.h"


/* ************************************************************************ */

#define cu8SELECTOR_CNT_CONF    ((uint8_t) 2U)

#define cu16SYSTEM_TIMER_OVFS_PER_S    ((uint16_t) 2U)

static OperatingMode_t enuCurrentMode = MODE_INIT;

Elevator_t strElevator = {0}; // Main elevator state

OperatingMode_t enuPreviousMode = MODE_NORMAL;

uint8_t u8SelectorCount = 0, u8SelectorPoleCnt = 0;

uint8_t u8AccessFlag = 0;
uint16_t u16EnteredPass = 0, u16StoredPass = 0, u16MasterPass = 0;

boolean segmentBlinkState = FALSE;

PinState_t enuStopState = STATE_LOW;
PinState_t enuSelState = STATE_LOW;
PinState_t enuFloorMState = STATE_LOW;
PinState_t enuSHKState = STATE_LOW;
PinState_t enuLockState = STATE_LOW;
PinState_t enuMUPState = STATE_LOW;
PinState_t enuMDNState = STATE_LOW;
PinState_t enuUPLState = STATE_LOW;
PinState_t enuDNLState = STATE_LOW;
PinState_t enuMNTState = STATE_LOW;
PinState_t enuOpenSigState = STATE_LOW;
PinState_t enuCloseSigState = STATE_LOW;
PinState_t enuOVLState = STATE_LOW;
PinState_t enuFLState = STATE_LOW;
PinState_t enuPHS1State = STATE_LOW;
PinState_t enuPHS2State = STATE_LOW;

volatile uint8_t blinkTimerOvf = 0;

volatile uint16_t u16SlowTimerCounter = 0;
volatile uint16_t u16FastTimerCounter = 0;
volatile uint16_t u16CamTimerCounter = 0;
volatile uint16_t u16ShkTimerCounter = 0;
volatile uint16_t u16StopTimerCounter = 0;
volatile uint16_t u16LightTimerCounter = 0;
volatile boolean bSlowTimerActive = FALSE;
volatile boolean bFastTimerActive = FALSE;
volatile boolean bCamTimerActive = FALSE;
volatile boolean bShkTimerActive = FALSE;
volatile boolean bStopTimerActive = FALSE;
volatile boolean bLightTimerActive = FALSE;
volatile boolean bSlowTimerExpired = FALSE;
volatile boolean bFastTimerExpired = FALSE;
volatile boolean bCamTimerExpired = FALSE;
volatile boolean bShkTimerExpired = FALSE;
volatile boolean bStopTimerExpired = FALSE;
volatile boolean bLightTimerExpired = FALSE;


Timer_cfg_t strBlinkTimer;

/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */

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

/**
 * @brief Set values in EEPROM to default
 * 
 */
static void vidResetDefaults(void);

/**
 * @brief Blink timer callback function
 *
 */
static void blinkTimerCBK(void);

/**
 * @brief Reads all sensors and updates their states
 *
 */
static void readAllSensors(void);

/**
 * @brief System timer callback function
 *
 */
static void systemTimerCBK(void);

/**
 * @brief Drops all pending elevator calls
 * This function iterates through all floors and resets their call states to CALL_NONE.
 * 
 */
static void vidDropCalls(void);

/**
 * @brief Checks if the call queue is empty
 * 
 * @return boolean TRUE if the call queue is empty, FALSE otherwise
 */
static boolean bIsCallQueueEmpty(void);

static void vidStartSlowTimer(void);
static void vidStopSlowTimer(void);
static boolean bIsSlowTimerExpired(void);

static void vidStartFastTimer(void);
static void vidStopFastTimer(void);
static boolean bIsFastTimerExpired(void);

static void vidStartCamTimer(void);
static void vidStopCamTimer(void);
static boolean bIsCamTimerExpired(void);

static void vidStartShkTimer(void);
static void vidStopShkTimer(void);
static boolean bIsShkTimerExpired(void);

static void vidStartStopTimer(void);
static void vidStopStopTimer(void);
static boolean bIsStopTimerExpired(void);

static void vidStopLightTimer(void);
static boolean bIsLightTimerExpired(void);

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
    uint8_t u8Index;

    elevator_hal_vidInit();
    
    elevator_hal_vidTimer_start(); // Start the timer

    updateMenuItems();

    Menu_Update();

    /* Init elevator state */
    strElevator.u8CurrentFloor = 0;
    strElevator.u8DestinationFloor = 0;
    strElevator.u8FloorCount = 0;
    strElevator.enuDirection = DIR_IDLE;
    strElevator.enuCollDir = COLLECTION_DOWN;
    strElevator.enuDoorState = DOOR_CLOSED;
    strElevator.enuOperatingMode = MODE_NORMAL;
    strElevator.bEmergencyStop = FALSE;

    for(u8Index = 0; u8Index < cu8MAX_FLOORS; u8Index++)
    {
        strElevator.aenuFloorCalls[u8Index] = CALL_NONE;
    }

    /* Update values from EEPROM */
    EEPROM_LoadValues();

    DBG_UART_INIT(9600);
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
    CallHandler_vidInit(&strElevator);

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
    strTimer.enumTimerIntMode = TIMER_INT_ENABLED;
    strTimer.enuTimerPre = TIMER_PRESCALER_64; // Example prescaler value
    strTimer.CBK_Ptr = systemTimerCBK; 
    Timer_Init(&strTimer);

    /* Init segment blink timer */
    strBlinkTimer.enuTimerCH = BLINK_TIMER_CHANNEL;
    strBlinkTimer.enumTimerIntMode = TIMER_INT_ENABLED;
    strBlinkTimer.enuTimerPre = TIMER_PRESCALER_1024; // Example prescaler value
    strBlinkTimer.CBK_Ptr = blinkTimerCBK; 
    Timer_Init(&strBlinkTimer);

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
        /* Update values from EEPROM */
        EEPROM_LoadValues();
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
    uint8_t u8Index = 0;

    DBG_PRINT_STRING("Operation loop started");
    
    while(1)
    {
        /* Sensors and mode checks */
        readAllSensors();
        /* Check for errors */
        if( (enuStopState == STATE_LOW) ||
            (enuUPLState == STATE_LOW) ||
            (enuDNLState == STATE_LOW) ||
            (enuOVLState == STATE_HIGH) ||
            (enuFLState == STATE_HIGH))
        {
            strElevator.enuOperatingMode = MODE_ERROR;
        }
        else
        {
            /* Check if phase sequence is used or not */
            if(strElevator.u8PhsSeq == 1U)
            {
                /* Phase sequence is used.
                 * Check for errors */
                if((enuPHS1State == STATE_HIGH) || (enuPHS2State == STATE_HIGH))
                {
                    strElevator.enuOperatingMode = MODE_ERROR;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }
        }

        switch(strElevator.enuOperatingMode)
        {
            case MODE_NORMAL:
                enuPreviousMode = MODE_NORMAL;
                /* Check wheather operating in normal mode or maintenance */
                if(enuMNTState == STATE_HIGH)
                {
                    /* Maintenance mode */
                    strElevator.enuOperatingMode = MODE_MAINTENANCE;
                    /* Activate mnt relay */
                    RelayManager_vidActivateRelay(RELAY_MNT);
                    /* turn cabin light on */
                    RelayManager_vidActivateRelay(RELAY_LIGHT);
                }
                else
                {
                    /* Normal mode */
                    /* Display current floor on 7-seg */
                    SegmentDriver_vidWrite(strElevator.u8CurrentFloor);

                    /* Display info on LCD */
                    LCD_SetCursor(0, 0);
                    LCD_WriteString("C: ");
                    LCD_send_int(strElevator.u8CurrentFloor, 2);
                    LCD_SetCursor(1, 0);
                    LCD_WriteString("D: ");


                    /* Check call queue */

                    CallHandler_vidGetCall();
                    
                    LCD_SetCursor(1, 3);
                    LCD_send_int(strElevator.u8DestinationFloor, 2);

                    /* restore each led state */
                    LEDController_vidProcess();

                    /* Process calls */
                    if(bIsCallQueueEmpty() == TRUE)
                    {
                        /* Call queue is empty */
                        strElevator.enuDirection = DIR_IDLE;
                    }
                    else
                    {
                        /* Process call queue */
                    }

                    if((strElevator.u8CurrentFloor == strElevator.u8DestinationFloor) && (strElevator.enuDirection != DIR_IDLE))
                    {
                        strElevator.enuDirection = DIR_SLOWING;
                    }

                    if(strElevator.enuDirection == DIR_SLOWING)
                    {
                        if(enuFloorMState == STATE_HIGH)
                        {
                            strElevator.enuDirection = DIR_STOPPING;
                        }
                    }

                    /* Movement action required ? */
                    if(strElevator.enuDirection == DIR_UP)
                    {
                        /* Move elevator up */
                        RelayManager_vidActivateRelay(RELAY_HS);
                        RelayManager_vidActivateRelay(RELAY_UP);
                    }
                    else if(strElevator.enuDirection == DIR_DOWN)
                    {
                        /* Move elevator down */
                        RelayManager_vidActivateRelay(RELAY_HS);
                        RelayManager_vidActivateRelay(RELAY_DN);
                    }
                    else if(strElevator.enuDirection == DIR_SLOWING)
                    {
                        /* Stop elevator */
                        RelayManager_vidDeActivateRelay(RELAY_HS);
                        RelayManager_vidActivateRelay(RELAY_LS);
                    }
                    else if(strElevator.enuDirection == DIR_STOPPING)
                    {
                        /* Stop elevator */
                        RelayManager_vidDeActivateRelay(RELAY_HS);
                        RelayManager_vidDeActivateRelay(RELAY_LS);
                        RelayManager_vidDeActivateRelay(RELAY_UP);
                        RelayManager_vidDeActivateRelay(RELAY_DN);

                        /* Indicate floor call has been served */
                        strElevator.aenuFloorCalls[strElevator.u8DestinationFloor] = CALL_NONE;
                        LEDController_vidSetState(strElevator.u8DestinationFloor, LED_STATE_OFF);
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                break;
            case MODE_MAINTENANCE:
                enuPreviousMode = MODE_MAINTENANCE;
                /* Check wheather operating in normal mode or maintenance */
                if(enuMNTState == STATE_LOW)
                {
                    /* Normal mode */
                    strElevator.enuOperatingMode = MODE_NORMAL;

                    /* Stop elevator */
                    strElevator.enuDirection = DIR_IDLE;
                    RelayManager_vidDeActivateRelay(RELAY_UP);
                    RelayManager_vidDeActivateRelay(RELAY_DN);
                    RelayManager_vidDeActivateRelay(RELAY_LS);
                    RelayManager_vidDeActivateRelay(RELAY_HS);

                    /* Activate light timer */
                    ElevatorController_vidStartLightTimer();

                    /* De-activate mnt relay */
                    RelayManager_vidDeActivateRelay(RELAY_MNT);

                    LCD_SetCursor(1, 0);
                    LCD_WriteString("                ");
                }
                else
                {
                    /* Maintenance mode */
                    /* Display current floor and pole cnt */
                    LCD_SetCursor(0, 0);
                    LCD_WriteString("C: ");
                    LCD_send_int((uint16_t)strElevator.u8CurrentFloor, 2);
                    
                    /* Start segment blink timer */
                    Timer_Start(BLINK_TIMER_CHANNEL);
                    
                    if(segmentBlinkState)
                    {
                        SegmentDriver_vidWrite(strElevator.u8CurrentFloor);
                    }
                    else
                    {
                        SegmentDriver_vidWrite(cu8MNT_ERROR);
                    }
                    /* Check if movement is required */
                    if((enuUPLState == STATE_HIGH) && (enuDNLState == STATE_HIGH))
                    {
                        if((enuMUPState == STATE_HIGH) && (enuMDNState == STATE_LOW))
                        {
                            /* Move up */
                            strElevator.enuDirection = DIR_UP;
                            RelayManager_vidActivateRelay(RELAY_UP);
                            if(strElevator.u8MntSpeed == 0)
                            {
                                /* Slow speed */
                                RelayManager_vidActivateRelay(RELAY_LS);
                            }
                            else
                            {
                                /* High speed */
                                RelayManager_vidActivateRelay(RELAY_HS);
                            }
                        }
                        else if((enuMUPState == STATE_LOW) && (enuMDNState == STATE_HIGH))
                        {
                            /* Move down */
                            strElevator.enuDirection = DIR_DOWN;
                            RelayManager_vidActivateRelay(RELAY_DN);
                            if(strElevator.u8MntSpeed == 0)
                            {
                                /* Slow speed */
                                RelayManager_vidActivateRelay(RELAY_LS);
                            }
                            else
                            {
                                /* High speed */
                                RelayManager_vidActivateRelay(RELAY_HS);
                            }
                        }
                        else
                        {
                            /* Stop elevator */
                            strElevator.enuDirection = DIR_IDLE;
                            RelayManager_vidDeActivateRelay(RELAY_UP);
                            RelayManager_vidDeActivateRelay(RELAY_DN);
                            RelayManager_vidDeActivateRelay(RELAY_LS);
                            RelayManager_vidDeActivateRelay(RELAY_HS);
                        }
                    }
                }
                break;
            case MODE_ERROR:
                /* Stop elevator */
                strElevator.enuDirection = DIR_IDLE;
                RelayManager_vidDeActivateRelay(RELAY_UP);
                RelayManager_vidDeActivateRelay(RELAY_DN);
                RelayManager_vidDeActivateRelay(RELAY_LS);
                RelayManager_vidDeActivateRelay(RELAY_HS);

                /* Drop all calls */
                vidDropCalls();
                LEDController_vidSetPatAllOff();
                
                /* Start segment blink timer */
                Timer_Start(BLINK_TIMER_CHANNEL);
                if(segmentBlinkState)
                {
                    SegmentDriver_vidWrite(strElevator.u8CurrentFloor);
                }
                else
                {
                    /* Display error */
                    LCD_SetCursor(1, 0);
                    if(enuStopState == STATE_LOW)
                    {
                        LCD_WriteString("STOP ERROR");
                        SegmentDriver_vidWrite(cu8STOP_ERROR);
                    }
                    else if(enuUPLState == STATE_LOW)
                    {
                        LCD_WriteString("UPL  ERROR");
                        SegmentDriver_vidWrite(cu8LIMIT_ERROR);
                    }
                    else if(enuDNLState == STATE_LOW)
                    {
                        LCD_WriteString("DNL  ERROR");
                        SegmentDriver_vidWrite(cu8LIMIT_ERROR);
                    }
                    else if(enuOVLState == STATE_HIGH)
                    {
                        LCD_WriteString("OVL  ERROR");
                        SegmentDriver_vidWrite(cu8OVL_ERROR);
                    }
                    else if(enuFLState == STATE_HIGH)
                    {
                        LCD_WriteString("FL   ERROR");
                        SegmentDriver_vidWrite(cu8FL_ERROR);
                    }
                    else if(enuPHS1State == STATE_HIGH)
                    {
                        LCD_WriteString("PHS1 ERROR");
                        SegmentDriver_vidWrite(cu8PHS_ERROR);
                    }
                    else if(enuPHS2State == STATE_HIGH)
                    {
                        LCD_WriteString("PHS2 ERROR");
                        SegmentDriver_vidWrite(cu8PHS_ERROR);
                    }
                    else
                    {
                        LCD_SetCursor(1, 0);
                        LCD_WriteString("                ");
                        if(enuPreviousMode == MODE_MAINTENANCE)
                        {
                            strElevator.enuOperatingMode = MODE_MAINTENANCE;
                        }
                        else
                        {
                            strElevator.enuOperatingMode = MODE_NORMAL;
                        }
                    }
                }
                break;
            default:
                // Handle unexpected operating mode
                break;

        }

        #ifdef TEST_
        if(strElevator.enuOperatingMode == MODE_NORMAL)
        {
            LCD_SetCursor(0, 13);
            LCD_WriteString("NRM");
        }
        else if(strElevator.enuOperatingMode == MODE_MAINTENANCE)
        {
            LCD_SetCursor(0, 13);
            LCD_WriteString("MNT");
        }
        else
        {
            /* Do nothing */
        }
        
        if(strElevator.enuDirection == DIR_UP)
        {
            LCD_SetCursor(1, 13);
            LCD_WriteString("UP ");
        }
        else if(strElevator.enuDirection == DIR_DOWN)
        {
            LCD_SetCursor(1, 13);
            LCD_WriteString("DN ");
        }
        else if(strElevator.enuDirection == DIR_STOPPING)
        {
            LCD_SetCursor(1, 13);
            LCD_WriteString("STP");
        }
        else if(strElevator.enuDirection == DIR_SLOWING)
        {
            LCD_SetCursor(1, 13);
            LCD_WriteString("SLW");
        }
        else
        {
            LCD_SetCursor(1, 13);
            LCD_WriteString("IDL");
        }
        #endif // TEST_

        /* Counting floors */
        if( (strElevator.enuOperatingMode == MODE_NORMAL) ||
            (strElevator.enuOperatingMode == MODE_MAINTENANCE))
            {
                /* debounce selsctor */
                if(enuSelState == STATE_HIGH)
                {
                    if(u8SelectorCount < cu8SELECTOR_CNT_CONF)
                    {
                        u8SelectorCount++;
                    }
                    else
                    {
                        u8SelectorCount = 0;
                        u8SelectorPoleCnt++;
                    }
                }
                if(strElevator.enuDirection == DIR_UP)
                {
                    if(strElevator.u8CurrentFloor < strElevator.u8FloorCount)
                    {
                        if(u8SelectorPoleCnt >= 2)
                        {
                            strElevator.u8CurrentFloor++;
                            u8SelectorPoleCnt = 0;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                else if(strElevator.enuDirection == DIR_DOWN)
                {
                    if(strElevator.u8CurrentFloor > 0)
                    {
                        if(u8SelectorPoleCnt >= 2)
                        {
                            strElevator.u8CurrentFloor--;
                            u8SelectorPoleCnt = 0;
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                    else
                    {
                        /* Do nothing */
                    }
                }
                else
                {
                    /* Do nothing */
                }
            }
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
    return strElevator.u8CurrentFloor;
}

/**
 * @brief Retrieves the selector pole count of the elevator.
 * 
 * @return uint8_t 
 */
uint8_t ElevatorController_u8GetSelectorCnt(void)
{
    return u8SelectorPoleCnt;
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

static void vidResetDefaults(void)
{
	/* RESET_DEFAULT_VALUES */
	(void) EEPROM_u8Update(cu8SLOW_TIMER_EE_ADD, cu8SLOW_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8FAST_TIMER_EE_ADD, cu8FAST_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CAM_TIMER_EE_ADD, cu8CAM_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8STOP_TIMER_EE_ADD, cu8STOP_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8LIGHT_TIMER_EE_ADD, cu8LIGHT_TIMER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8DOOR_NUMBER_EE_ADD, cu8DOOR_NUMBER_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8COLLECTION_DIR_EE_ADD, cu8COLLECTION_DIR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8MNT_SPEED_EE_ADD, cu8MNT_SPEED_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8DOOR_OPTIONS_EE_ADD, cu8DOOR_OPTIONS_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8PARKING_FLOOR_EE_ADD, cu8PARKING_FLOOR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CAM_FAIL_CNT_EE_ADD, cu8CAM_FAIL_CNT_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CABIN_PW_EE_ADD, cu8CABIN_PW_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8PHS_SEQ_EE_ADD, cu8PHS_SEQ_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8PASSWORD_L_EE_ADD, cu8PASSWORD_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8PASSWORD_H_EE_ADD, cu8PASSWORD_H_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CURRENT_FLOOR_EE_ADD, cu8CURRENT_FLOOR_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CURRENT_TRAVEL_L_EE_ADD, cu8CURRENT_TRAVEL_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8CURRENT_TRAVEL_H_EE_ADD, cu8CURRENT_TRAVEL_H_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8STORED_TRAVEL_EE_ADD, cu8STORED_TRAVEL_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8STORED_TRAVEL_USAGE_EE_ADD, cu8STORED_TRAVEL_USAGE_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8MASTER_PASSWORD_L_EE_ADD, cu8MASTER_PASSWORD_L_DEF_VALUE);
    _delay_ms(20);
	(void) EEPROM_u8Update(cu8MASTER_PASSWORD_H_EE_ADD, cu8MASTER_PASSWORD_H_DEF_VALUE);
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
    (void) EEPROM_u8Read(cu8COLLECTION_DIR_EE_ADD, (uint8_t*)&strElevator.enuCollDir);
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

/**
 * @brief Reads all sensors and updates their states
 * 
 */
static void readAllSensors(void)
{
    SensorManager_stdReadSensor(FLOOR_M, &enuFloorMState);
    SensorManager_stdReadSensor(SELECTOR, &enuSelState);
    SensorManager_stdReadSensor(STOP_SEN, &enuStopState);
    SensorManager_stdReadSensor(SHK, &enuSHKState);
    SensorManager_stdReadSensor(LOCK, &enuLockState);
    SensorManager_stdReadSensor(UP_LIMIT, &enuUPLState);
    SensorManager_stdReadSensor(DN_LIMIT, &enuDNLState);
    SensorManager_stdReadSensor(MNT_SEN, &enuMNTState);
    SensorManager_stdReadSensor(MNT_UP, &enuMUPState);
    SensorManager_stdReadSensor(MNT_DN, &enuMDNState);
    SensorManager_stdReadSensor(OPEN_DOOR, &enuOpenSigState);
    SensorManager_stdReadSensor(OVER_LOAD, &enuOVLState);
    SensorManager_stdReadSensor(CLOSE_DOOR, &enuCloseSigState);
    SensorManager_stdReadSensor(FULL_LOAD, &enuFLState);
    SensorManager_stdReadSensor(PHASE_1, &enuPHS1State);
    SensorManager_stdReadSensor(PHASE_2, &enuPHS2State);
}

/**
 * @brief Drops all pending elevator calls
 * This function iterates through all floors and resets their call states to CALL_NONE.
 * 
 */
static void vidDropCalls(void)
{
    uint8_t index;
    for(index = 0; index < strElevator.u8FloorCount; index++)
    {
        strElevator.aenuFloorCalls[index] = CALL_NONE;
    }
}

/**
 * @brief Checks if the call queue is empty
 * 
 * @return boolean TRUE if the call queue is empty, FALSE otherwise
 */
static boolean bIsCallQueueEmpty(void)
{
    uint8_t u8Index = 0;

    for(u8Index = 0; u8Index < strElevator.u8FloorCount; u8Index++)
    {
        if(strElevator.aenuFloorCalls[u8Index] != CALL_NONE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief Blink timer callback function
 * 
 */
static void blinkTimerCBK(void)
{
    blinkTimerOvf++;
    if (blinkTimerOvf >= cu8BLINK_TIMER_OVFS)
    {
        blinkTimerOvf = 0;
        segmentBlinkState = !segmentBlinkState;
    }

}


/**
 * @brief System timer callback function
 *
 */
static void systemTimerCBK(void)
{
    if (bSlowTimerActive) {
        u16SlowTimerCounter++;
        if (u16SlowTimerCounter >= (strElevator.u8SlowTimer * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bSlowTimerExpired = TRUE;
            bSlowTimerActive = FALSE;
            u16SlowTimerCounter = 0;
        }
    }

    if (bFastTimerActive) {
        u16FastTimerCounter++;
        if (u16FastTimerCounter >= (strElevator.u8FastTimer * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bFastTimerExpired = TRUE;
            bFastTimerActive = FALSE;
            u16FastTimerCounter = 0;
        }
    }

    if (bCamTimerActive) {
        u16CamTimerCounter++;
        if (u16CamTimerCounter >= (strElevator.u8CamTimer * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bCamTimerExpired = TRUE;
            bCamTimerActive = FALSE;
            u16CamTimerCounter = 0;
        }
    }

    if (bShkTimerActive) {
        u16ShkTimerCounter++;
        if (u16ShkTimerCounter >= (100 * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bShkTimerExpired = TRUE;
            bShkTimerActive = FALSE;
            u16ShkTimerCounter = 0;
        }
    }

    if (bStopTimerActive) {
        u16StopTimerCounter++;
        if (u16StopTimerCounter >= (strElevator.u8StopTimer * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bStopTimerExpired = TRUE;
            bStopTimerActive = FALSE;
            u16StopTimerCounter = 0;
        }
    }

    if (bLightTimerActive) {
        u16LightTimerCounter++;
        if (u16LightTimerCounter >= (((uint16_t)strElevator.u8LightTimer) * cu16SYSTEM_TIMER_OVFS_PER_S)) {
            bLightTimerExpired = TRUE;
            bLightTimerActive = FALSE;
            u16LightTimerCounter = 0;
            /* De-activate light relay */
            RelayManager_vidDeActivateRelay(RELAY_LIGHT);
        }
    }
}

/* ************************************************************************ */
/* Timer Control Functions Implementation                                   */
/* ************************************************************************ */

/**
 * @brief Start the slow timer
 *
 */
static void vidStartSlowTimer(void)
{
    u16SlowTimerCounter = 0;
    bSlowTimerExpired = FALSE;
    bSlowTimerActive = TRUE;
}

/**
 * @brief Stop the slow timer
 *
 */
static void vidStopSlowTimer(void)
{
    bSlowTimerActive = FALSE;
    u16SlowTimerCounter = 0;
}

/**
 * @brief Check if slow timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsSlowTimerExpired(void)
{
    return bSlowTimerExpired;
}

/**
 * @brief Start the fast timer
 *
 */
static void vidStartFastTimer(void)
{
    u16FastTimerCounter = 0;
    bFastTimerExpired = FALSE;
    bFastTimerActive = TRUE;
}

/**
 * @brief Stop the fast timer
 *
 */
static void vidStopFastTimer(void)
{
    bFastTimerActive = FALSE;
    u16FastTimerCounter = 0;
}

/**
 * @brief Check if fast timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsFastTimerExpired(void)
{
    return bFastTimerExpired;
}

/**
 * @brief Start the camera timer
 *
 */
static void vidStartCamTimer(void)
{
    u16CamTimerCounter = 0;
    bCamTimerExpired = FALSE;
    bCamTimerActive = TRUE;
}

/**
 * @brief Stop the camera timer
 *
 */
static void vidStopCamTimer(void)
{
    bCamTimerActive = FALSE;
    u16CamTimerCounter = 0;
}

/**
 * @brief Check if camera timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsCamTimerExpired(void)
{
    return bCamTimerExpired;
}

/**
 * @brief Start the shake timer
 *
 */
static void vidStartShkTimer(void)
{
    u16ShkTimerCounter = 0;
    bShkTimerExpired = FALSE;
    bShkTimerActive = TRUE;
}

/**
 * @brief Stop the shake timer
 *
 */
static void vidStopShkTimer(void)
{
    bShkTimerActive = FALSE;
    u16ShkTimerCounter = 0;
}

/**
 * @brief Check if shake timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsShkTimerExpired(void)
{
    return bShkTimerExpired;
}

/**
 * @brief Start the stop timer
 *
 */
static void vidStartStopTimer(void)
{
    u16StopTimerCounter = 0;
    bStopTimerExpired = FALSE;
    bStopTimerActive = TRUE;
}

/**
 * @brief Stop the stop timer
 *
 */
static void vidStopStopTimer(void)
{
    bStopTimerActive = FALSE;
    u16StopTimerCounter = 0;
}

/**
 * @brief Check if stop timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsStopTimerExpired(void)
{
    return bStopTimerExpired;
}

/**
 * @brief Start the light timer
 *
 */
void ElevatorController_vidStartLightTimer(void)
{
    u16LightTimerCounter = 0;
    bLightTimerExpired = FALSE;
    bLightTimerActive = TRUE;
}

/**
 * @brief Stop the light timer
 *
 */
static void vidStopLightTimer(void)
{
    bLightTimerActive = FALSE;
    u16LightTimerCounter = 0;
}

/**
 * @brief Check if light timer has expired
 *
 * @return boolean TRUE if expired, FALSE otherwise
 */
static boolean bIsLightTimerExpired(void)
{
    return bLightTimerExpired;
}

/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
