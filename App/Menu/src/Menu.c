
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

#define Menu_c

/* ************************************************************************ */
/* Header Inclusions                                                        */
/* ************************************************************************ */

/* standard and platform dependent types ********************************** */

#include <stdio.h>
#include <util/delay.h>
#include "std_types.h"

/* other components of the project **************************************** */

#include "ButtonDriver.h"
#include "lcd_config.h"
#include "EepD.h"
#include "Timer.h"
#include "helperFunc.h"

/* own header inclusions ************************************************** */

/* access to its own exports */
#include "Menu.h"

/* access to the modul's local configuration */
#include "Menu_lcfg.h"


static MenuItem_t menuItems[MENU_ITEM_COUNT];
static MenuItemId_t currentItemId = SLOW_TIMER_MID;
static MenuMode_t currentMode = MENU_MODE_NAVIGATE;
volatile uint8_t blinkState = 0;
volatile uint16_t blinkCounter = 0;
static uint8_t itemCount = 0;
//static 


/* ************************************************************************ */
/* ************************************************************************ */
/*                     LOCAL FUNCTIONS PROTOTYPES                           */
/* ************************************************************************ */
/* ************************************************************************ */
static void Menu_HandleBlinkDisplay(void);
static void UpdateBlinkCounter(void);
static void Menu_DisplayCurrentItem(void);

/* ************************************************************************ */
/* ************************************************************************ */
/*                          EXPORTED FUNCTIONS                              */
/* ************************************************************************ */
/* ************************************************************************ */
/* Basic Services                                                           */
/* ************************************************************************ */

/* ************************************************************************ */

void Menu_Init(void) {
    my_memset(menuItems, 0, sizeof(menuItems));

    /* Initialize menuBlink timer */
    Timer_cfg_t strTimer;
    strTimer.enuTimerCH = MENU_BLINK_HW_TIMER_CHANNEL;
    strTimer.enumTimerIntMode = TIMER_INT_ENABLED;
    strTimer.enuTimerPre = TIMER_PRESCALER_1024; // Example prescaler value
    strTimer.CBK_Ptr = UpdateBlinkCounter; // No callback function for now
    Timer_Init(&strTimer);

    currentItemId = SLOW_TIMER_MID;
    currentMode = MENU_MODE_NAVIGATE;
    blinkState = 0;
    blinkCounter = 0;
    itemCount = 0;
}

void Menu_SetItemData(MenuItemId_t itemId, const char* label, uint8_t value, uint8_t minValue, uint8_t maxValue, uint8_t eepromAddress) {
    if (itemId < MENU_ITEM_COUNT) {
        menuItems[itemId].label = label;
        menuItems[itemId].value = value;
        menuItems[itemId].minValue = minValue;
        menuItems[itemId].maxValue = maxValue;
        menuItems[itemId].eepromAddress = eepromAddress;
        
        if (itemId >= itemCount) {
            itemCount = itemId + 1;
        }
    }
}

void Menu_UpdateItemValue(MenuItemId_t itemId, uint8_t value) {
    if (itemId < MENU_ITEM_COUNT) {
        menuItems[itemId].value = value;
    }
}

static void Menu_DisplayCurrentItem(void) {
    char buffer[17];

    LCD_Clear();

    if (currentItemId < itemCount) {
        LCD_SetCursor(0, 0);
        LCD_WriteString((char*)menuItems[currentItemId].label);

        Menu_HandleBlinkDisplay();
    }
}

static void Menu_HandleBlinkDisplay(void)
{
    char buffer[17];

    LCD_SetCursor(1, 0);
    if (currentMode == MENU_MODE_EDIT && blinkState == 0) {
        snprintf(buffer, sizeof(buffer), "    ");
    } else {
        snprintf(buffer, sizeof(buffer), "%d", menuItems[currentItemId].value);
    }
    LCD_WriteString(buffer);
}

static void Menu_HandleNavigateMode(void) {
    if (ButtonDriver_bIsPressed(BTN_UP)) {
        if (currentItemId < (itemCount - 1)) {
            currentItemId++;
        } else {
            currentItemId = 0;
        }
        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_UP));
    }

    if (ButtonDriver_bIsPressed(BTN_DN)) {
        if (currentItemId > 0) {
            currentItemId--;
        } else {
            currentItemId = itemCount - 1;
        }
        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_DN));
    }

    if (ButtonDriver_bIsPressed(BTN_OK)) {
        currentMode = MENU_MODE_EDIT;
        blinkState = 1;
        blinkCounter = 0;
        Timer_Start(MENU_BLINK_HW_TIMER_CHANNEL);
        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_OK));
    }
}

static void Menu_HandleEditMode(void) {
    if (ButtonDriver_bIsPressed(BTN_UP)) {
        if (menuItems[currentItemId].value < menuItems[currentItemId].maxValue) {
            menuItems[currentItemId].value++;
        }
        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_UP));
    }

    if (ButtonDriver_bIsPressed(BTN_DN)) {
        if (menuItems[currentItemId].value > menuItems[currentItemId].minValue) {
            menuItems[currentItemId].value--;
        }
        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_DN));
    }

    if (ButtonDriver_bIsPressed(BTN_OK)) {
        currentMode = MENU_MODE_NAVIGATE;
        blinkState = 0;
        Timer_Stop(MENU_BLINK_HW_TIMER_CHANNEL);

        (void) EEPROM_u8Update(menuItems[currentItemId].eepromAddress, menuItems[currentItemId].value);
        _delay_ms(20);

        Menu_DisplayCurrentItem();
        while (ButtonDriver_bIsPressed(BTN_OK));
    }
}

void Menu_Task(void) {
    static uint8_t lastBlinkState = 0;
    if (currentMode == MENU_MODE_EDIT) {
        if (lastBlinkState != blinkState) {
            lastBlinkState = blinkState;
            Menu_HandleBlinkDisplay();
        }
        Menu_HandleEditMode();
    }
    else
    {
        Menu_HandleNavigateMode();
    }
}

void Menu_Update(void)
{
    Menu_DisplayCurrentItem();
}


/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           LOCAL FUNCTIONS                                */
/* ************************************************************************ */
/* ************************************************************************ */

/**
 * @brief Updates the blink counter used for menu item blinking
 * 
 */
static void UpdateBlinkCounter(void)
{
    blinkCounter++;
    if (blinkCounter >= MENU_BLINK_PERIOD_MS) {
        blinkCounter = 0;
        blinkState = !blinkState;
    }
}

/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
