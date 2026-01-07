#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <stdint.h>

#define I2C_LCD_ADDRESS     0x20

#define LCD_BACKLIGHT       0x08
#define LCD_ENABLE          0x04
#define LCD_RW              0x02
#define LCD_RS              0x01

#define LCD_CMD_CLEAR       0x01
#define LCD_CMD_HOME        0x02
#define LCD_CMD_ENTRY_MODE  0x06
#define LCD_CMD_DISPLAY_ON  0x0C
#define LCD_CMD_DISPLAY_OFF 0x08
#define LCD_CMD_CURSOR_ON   0x0E
#define LCD_CMD_CURSOR_OFF  0x0C
#define LCD_CMD_BLINK_ON    0x0F
#define LCD_CMD_BLINK_OFF   0x0E
#define LCD_CMD_4BIT_MODE   0x28
#define LCD_CMD_8BIT_MODE   0x38
#define LCD_CMD_SET_DDRAM   0x80

void I2C_LCD_Init(void);
void I2C_LCD_Clear(void);
void I2C_LCD_Home(void);
void I2C_LCD_SetCursor(uint8_t row, uint8_t col);
void I2C_LCD_WriteChar(char c);
void I2C_LCD_WriteString(const char* str);
void I2C_LCD_WriteCommand(uint8_t cmd);
void I2C_LCD_WriteData(uint8_t data);
void I2C_LCD_Backlight(uint8_t state);
void I2C_LCD_Display(uint8_t state);
void I2C_LCD_Cursor(uint8_t state);
void I2C_LCD_Blink(uint8_t state);

#endif
