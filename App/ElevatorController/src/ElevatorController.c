

#define ElevatorController_c

#include "Std_Types.h"

#include <util/delay.h>
#include "ElevatorController.h"
#include "ElevatorController_lcfg.h"
#include "Timer.h"

#include "SystemConfig.h"

#include "RelayManager.h"
#include "LEDController.h"
#include "CallHandler.h"
#include "SegmentDriver.h"
#include "lcd_config.h"
#include "ButtonDriver.h"
#include "SensorManager.h"
#include "MotionController.h"
#include "EepD.h"


static OperatingMode_t enuCurrentMode = MODE_INIT;

uint8_t u8AccessFlag = 0;
uint16_t u16EnteredPass = 0, u16StoredPass = 0, u16MasterPass = 0;
uint8_t u8SlowTimer = 0;

// 1. Safety checks
static boolean vidCheckEmergencyConditions(Elevator_t* pstrElevator)
{
    PinState_t enuStopState;
    // Check emergency stop button, door sensors, etc.
    (void) SensorManager_stdReadSensor(STOP_SEN, &enuStopState);
    if(enuStopState == STATE_LOW)
    {
        pstrElevator->bEmergencyStop = TRUE;
        elevator_hal_vidMotor_stop();
        return TRUE;
    }
    return FALSE;
}

// 2. Process button inputs
static void vidProcessButtonInputs(Elevator_t* pstrElevator)
{
    uint8_t u8Floor;
    
    // Check floor buttons (0 to MAX_FLOORS-1)
    for(u8Floor = 0; u8Floor < cu8MAX_FLOORS; u8Floor++)
    {
        if(ButtonDriver_bIsPressed(u8Floor)) // Assuming floor buttons
        {
            //CallHandler_bRegister_call(u8Floor, CALL_INTERNAL, DIR_UP);
        }
    }
}

// 3. Update current floor based on sensors
static void vidUpdateCurrentFloor(Elevator_t* pstrElevator)
{
    uint8_t u8Floor;
    
    for(u8Floor = 0; u8Floor < cu8MAX_FLOORS; u8Floor++)
    {
        //if(SensorManager_bReadFloorSensor(u8Floor))
        {
            if(pstrElevator->u8CurrentFloor != u8Floor)
            {
                pstrElevator->u8CurrentFloor = u8Floor;
                // Clear calls for this floor if we're stopping
                if(pstrElevator->enuDirection != DIR_IDLE)
                {
                    //CallHandler_vidClear_call(u8Floor);
                }
            }
            break;
        }
    }
}

// 4. Handle idle state
static void vidHandleIdleState(Elevator_t* pstrElevator)
{
    /*
    // Check if there are any calls
    if(CallHandler_bHas_calls_above(pstrElevator->u8CurrentFloor))
    {
        pstrElevator->enuDirection = DIR_UP;
        elevator_hal_vidMotor_up();
    }
    else if(CallHandler_bHas_calls_below(pstrElevator->u8CurrentFloor))
    {
        pstrElevator->enuDirection = DIR_DOWN;
        elevator_hal_vidMotor_down();
    }
    // Otherwise stay idle
    */
}

// 5. Handle upward movement
static void vidHandleUpMovement(Elevator_t* pstrElevator)
{
    uint8_t u8NextFloor = 0;//CallHandler_u8Get_next_floor(pstrElevator->u8CurrentFloor, DIR_UP);
    
    if(u8NextFloor == pstrElevator->u8CurrentFloor)
    {
        // Reached target floor
        elevator_hal_vidMotor_stop();
        pstrElevator->enuDirection = DIR_IDLE;
        pstrElevator->enuDoorState = DOOR_OPENING;
        pstrElevator->u16DoorTimer = elevator_hal_u16Get_time_ms();
    }
    // Continue moving up
}

// 6. Handle downward movement  
static void vidHandleDownMovement(Elevator_t* pstrElevator)
{
    uint8_t u8NextFloor = 0;//CallHandler_u8Get_next_floor(pstrElevator->u8CurrentFloor, DIR_DOWN);
    
    if(u8NextFloor == pstrElevator->u8CurrentFloor)
    {
        // Reached target floor
        elevator_hal_vidMotor_stop();
        pstrElevator->enuDirection = DIR_IDLE;
        pstrElevator->enuDoorState = DOOR_OPENING;
        pstrElevator->u16DoorTimer = elevator_hal_u16Get_time_ms();
    }
    // Continue moving down
}

// 7. Door control
static void vidProcessDoorControl(Elevator_t* pstrElevator)
{
    uint16_t u16CurrentTime = elevator_hal_u16Get_time_ms();
    
    switch(pstrElevator->enuDoorState)
    {
        case DOOR_OPENING:
            elevator_hal_vidDoor_open();
            pstrElevator->enuDoorState = DOOR_OPEN;
            pstrElevator->u16DoorTimer = u16CurrentTime;
            break;
            
        case DOOR_OPEN:
            // Keep door open for configured time
            if(u16CurrentTime - pstrElevator->u16DoorTimer > DOOR_OPEN_TIME_MS)
            {
                pstrElevator->enuDoorState = DOOR_CLOSING;
            }
            break;
            
        case DOOR_CLOSING:
            elevator_hal_vidDoor_close();
            pstrElevator->enuDoorState = DOOR_CLOSED;
            break;
            
        case DOOR_CLOSED:
            // Ready for movement
            break;
    }
}

// 8. Update displays
static void vidUpdateDisplays(Elevator_t* pstrElevator)
{
    elevator_hal_vidDisplay_floor(pstrElevator->u8CurrentFloor);
    
    switch(pstrElevator->enuDirection)
    {
        case DIR_UP:
            elevator_hal_vidDisplay_status("UP  ");
            break;
        case DIR_DOWN:
            elevator_hal_vidDisplay_status("DOWN");
            break;
        default:
            elevator_hal_vidDisplay_status("IDLE");
            break;
    }
}

/*
 * @brief Initialize the elevator controller.
 * @description This function sets up the elevator controller
 * by initializing the necessary hardware components and configurations.
*/
void ElevatorController_Init(void)
{
    elevator_hal_vidInit();

    elevator_hal_vidTimer_start(); // Start the timer
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

    /* Initialize system timer */
    Timer_cfg_t strTimer;
    strTimer.enuTimerCH = SYSTEM_TIMER_CHANNEL;
    strTimer.enumTimerIntMode = TIMER_INT_DISABLED;
    strTimer.enuTimerPre = TIMER_PRESCALER_1024; // Example prescaler value
    strTimer.CBK_Ptr = NULL; // No callback function for now
    Timer_Init(&strTimer);
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
	(void) EEPROM_u8Read(cu8SLOW_TIMER_EE_ADD, &u8SlowTimer);

    if(u8SlowTimer == 0xFF)
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
    //LCD_clear();
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
    static Elevator_t strElevator = {0}; // Main elevator state
    static uint16_t u16LastProcessTime = 0;
    uint16_t u16CurrentTime;
    
    // Initialize elevator state
    strElevator.u8CurrentFloor = 0;
    strElevator.enuDirection = DIR_IDLE;
    strElevator.enuDoorState = DOOR_CLOSED;
    strElevator.enuOperatingMode = MODE_NORMAL;
    strElevator.bEmergencyStop = FALSE;
    
    while(1)
    {
        u16CurrentTime = elevator_hal_u16Get_time_ms();
        
        // 1. SAFETY CHECKS
        if(vidCheckEmergencyConditions(&strElevator))
        {
            continue; // Skip normal operation if emergency
        }
        
        // 2. PROCESS BUTTON INPUTS
        vidProcessButtonInputs(&strElevator);
        
        // 3. UPDATE CURRENT FLOOR
        vidUpdateCurrentFloor(&strElevator);
        
        // 4. PROCESS CALLS
        //CallHandler_vidProcess(&strElevator);
        
        // 5. MAIN STATE MACHINE
        switch(strElevator.enuDirection)
        {
            case DIR_IDLE:
                vidHandleIdleState(&strElevator);
                break;
                
            case DIR_UP:
                vidHandleUpMovement(&strElevator);
                break;
                
            case DIR_DOWN:
                vidHandleDownMovement(&strElevator);
                break;
        }
        
        // 6. DOOR CONTROL
        vidProcessDoorControl(&strElevator);
        
        // 7. UPDATE DISPLAYS
        vidUpdateDisplays(&strElevator);
        
        // 8. PROCESS LEDS
        LEDController_vidProcess();
        
        // Small delay to prevent overwhelming the system
        _delay_ms(10);
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

            LCD_SetCursor(5, 1);
            //LCD_send_int(u16EnteredPass, 3);
            
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
                    LCD_SetCursor(0, 1);
                    LCD_WriteString("Access Granted");
                    _delay_ms(1000); // Display for 1 second
                }
                else
                {
                    u8AccessFlag = FALSE;
                    LCD_SetCursor(0, 1);
                    LCD_WriteString("Access Denied");
                    _delay_ms(1000); // Display for 1 second
                    LCD_SetCursor(0, 1);
                    LCD_WriteString("             ");
                }
            }
        }
        else
        {
            /* Display config menu */
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
	(void) EEPROM_u8Write(cu8FAST_TIMER_EE_ADD, cu8FAST_TIMER_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CAM_TIMER_EE_ADD, cu8CAM_TIMER_DEF_VALUE);
	(void) EEPROM_u8Write(cu8STOP_TIMER_EE_ADD, cu8STOP_TIMER_DEF_VALUE);
	(void) EEPROM_u8Write(cu8LIGHT_TIMER_EE_ADD, cu8LIGHT_TIMER_DEF_VALUE);
	(void) EEPROM_u8Write(cu8DOOR_NUMBER_EE_ADD, cu8DOOR_NUMBER_DEF_VALUE);
	(void) EEPROM_u8Write(cu8DISPLAY_EE_ADD, cu8DISPLAY_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_0_EE_ADD, cu8FLOOR_0_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_1_EE_ADD, cu8FLOOR_1_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_2_EE_ADD, cu8FLOOR_2_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_3_EE_ADD, cu8FLOOR_3_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_4_EE_ADD, cu8FLOOR_4_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_5_EE_ADD, cu8FLOOR_5_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_6_EE_ADD, cu8FLOOR_6_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_7_EE_ADD, cu8FLOOR_7_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_8_EE_ADD, cu8FLOOR_8_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_9_EE_ADD, cu8FLOOR_9_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_10_EE_ADD, cu8FLOOR_10_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_11_EE_ADD, cu8FLOOR_11_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_12_EE_ADD, cu8FLOOR_12_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_13_EE_ADD, cu8FLOOR_13_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_14_EE_ADD, cu8FLOOR_14_DEF_VALUE);
	(void) EEPROM_u8Write(cu8FLOOR_15_EE_ADD, cu8FLOOR_15_DEF_VALUE);
	(void) EEPROM_u8Write(cu8COLLECTION_DIR_EE_ADD, cu8COLLECTION_DIR_DEF_VALUE);
	(void) EEPROM_u8Write(cu8MNT_SPEED_EE_ADD, cu8MNT_SPEED_DEF_VALUE);
	(void) EEPROM_u8Write(cu8DOOR_OPTIONS_EE_ADD, cu8DOOR_OPTIONS_DEF_VALUE);
	(void) EEPROM_u8Write(cu8PARKING_FLOOR_EE_ADD, cu8PARKING_FLOOR_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CAM_FAIL_CNT_EE_ADD, cu8CAM_FAIL_CNT_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CABIN_PW_EE_ADD, cu8CABIN_PW_DEF_VALUE);
	(void) EEPROM_u8Write(cu8PHS_SEQ_EE_ADD, cu8PHS_SEQ_DEF_VALUE);
	(void) EEPROM_u8Write(cu8PASSWORD_L_EE_ADD, cu8PASSWORD_L_DEF_VALUE);
	(void) EEPROM_u8Write(cu8PASSWORD_H_EE_ADD, cu8PASSWORD_H_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CURRENT_FLOOR_EE_ADD, cu8CURRENT_FLOOR_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CURRENT_TRAVEL_L_EE_ADD, cu8CURRENT_TRAVEL_L_DEF_VALUE);
	(void) EEPROM_u8Write(cu8CURRENT_TRAVEL_H_EE_ADD, cu8CURRENT_TRAVEL_H_DEF_VALUE);
	(void) EEPROM_u8Write(cu8STORED_TRAVEL_EE_ADD, cu8STORED_TRAVEL_DEF_VALUE);
	(void) EEPROM_u8Write(cu8STORED_TRAVEL_USAGE_EE_ADD, cu8STORED_TRAVEL_USAGE_DEF_VALUE);
	(void) EEPROM_u8Write(cu8MASTER_PASSWORD_L_EE_ADD, cu8MASTER_PASSWORD_L_DEF_VALUE);
	(void) EEPROM_u8Write(cu8MASTER_PASSWORD_H_EE_ADD, cu8MASTER_PASSWORD_H_DEF_VALUE);
}
