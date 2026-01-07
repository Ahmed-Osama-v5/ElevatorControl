#include "i2c_lcd.h"
#include "soft_i2c.h"
#include <util/delay.h>

static uint8_t backlight_state = LCD_BACKLIGHT;

static void I2C_LCD_SendNibble(uint8_t nibble, uint8_t mode);
static void I2C_LCD_SendByte(uint8_t data, uint8_t mode);
static void I2C_LCD_PulseEnable(uint8_t data);

static void I2C_LCD_PulseEnable(uint8_t data) {
    uint8_t temp = data | LCD_ENABLE | backlight_state;
    SoftI2C_Write(I2C_LCD_ADDRESS, &temp, 1);
    _delay_us(1);
    
    temp = data & ~LCD_ENABLE | backlight_state;
    SoftI2C_Write(I2C_LCD_ADDRESS, &temp, 1);
    _delay_us(50);
}

static void I2C_LCD_SendNibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | backlight_state;
    SoftI2C_Write(I2C_LCD_ADDRESS, &data, 1);
    I2C_LCD_PulseEnable(data);
}

static void I2C_LCD_SendByte(uint8_t data, uint8_t mode) {
    I2C_LCD_SendNibble(data & 0xF0, mode);
    I2C_LCD_SendNibble((data << 4) & 0xF0, mode);
}

void I2C_LCD_Init(void) {
    
    /* set LCD pins as outputs */
    Dio_Cfg_t strDio = {0};
    strDio.enuGPIO = GPIOB;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = 0;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = GPIOB;
    strDio.u8Pin = 1;
    DIO_Init(&strDio);

    SoftI2C_Init();
    _delay_ms(50);
    
    I2C_LCD_SendNibble(0x30, 0);
    _delay_ms(5);
    I2C_LCD_SendNibble(0x30, 0);
    _delay_us(150);
    I2C_LCD_SendNibble(0x30, 0);
    _delay_us(150);
    I2C_LCD_SendNibble(0x20, 0);
    _delay_us(150);
    
    I2C_LCD_WriteCommand(LCD_CMD_4BIT_MODE);
    I2C_LCD_WriteCommand(LCD_CMD_DISPLAY_OFF);
    I2C_LCD_WriteCommand(LCD_CMD_CLEAR);
    _delay_ms(2);
    I2C_LCD_WriteCommand(LCD_CMD_ENTRY_MODE);
    I2C_LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
}

void I2C_LCD_WriteCommand(uint8_t cmd) {
    I2C_LCD_SendByte(cmd, 0);
    if (cmd == LCD_CMD_CLEAR || cmd == LCD_CMD_HOME) {
        _delay_ms(2);
    } else {
        _delay_us(50);
    }
}

void I2C_LCD_WriteData(uint8_t data) {
    I2C_LCD_SendByte(data, LCD_RS);
    _delay_us(50);
}

void I2C_LCD_Clear(void) {
    I2C_LCD_WriteCommand(LCD_CMD_CLEAR);
}

void I2C_LCD_Home(void) {
    I2C_LCD_WriteCommand(LCD_CMD_HOME);
}

void I2C_LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 0;
    I2C_LCD_WriteCommand(LCD_CMD_SET_DDRAM | (col + row_offsets[row]));
}

void I2C_LCD_WriteChar(char c) {
    I2C_LCD_WriteData((uint8_t)c);
}

void I2C_LCD_WriteString(const char* str) {
    while (*str) {
        I2C_LCD_WriteChar(*str++);
    }
}

void I2C_LCD_Backlight(uint8_t state) {
    if (state) {
        backlight_state = LCD_BACKLIGHT;
    } else {
        backlight_state = 0;
    }
    uint8_t data = backlight_state;
    SoftI2C_Write(I2C_LCD_ADDRESS, &data, 1);
}

void I2C_LCD_Display(uint8_t state) {
    if (state) {
        I2C_LCD_WriteCommand(LCD_CMD_DISPLAY_ON);
    } else {
        I2C_LCD_WriteCommand(LCD_CMD_DISPLAY_OFF);
    }
}

void I2C_LCD_Cursor(uint8_t state) {
    if (state) {
        I2C_LCD_WriteCommand(LCD_CMD_CURSOR_ON);
    } else {
        I2C_LCD_WriteCommand(LCD_CMD_CURSOR_OFF);
    }
}

void I2C_LCD_Blink(uint8_t state) {
    if (state) {
        I2C_LCD_WriteCommand(LCD_CMD_BLINK_ON);
    } else {
        I2C_LCD_WriteCommand(LCD_CMD_BLINK_OFF);
    }
}
