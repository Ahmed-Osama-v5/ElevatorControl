#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#define LCD_USE_I2C     0
#define LCD_USE_PARALLEL 1

#define LCD_DRIVER_TYPE  LCD_USE_PARALLEL

#if (LCD_DRIVER_TYPE == LCD_USE_I2C)
    #include "i2c_lcd.h"
    
    #define LCD_Init()              I2C_LCD_Init()
    #define LCD_Clear()             I2C_LCD_Clear()
    #define LCD_Home()              I2C_LCD_Home()
    #define LCD_SetCursor(r, c)     I2C_LCD_SetCursor(r, c)
    #define LCD_WriteChar(ch)       I2C_LCD_WriteChar(ch)
    #define LCD_WriteString(str)    I2C_LCD_WriteString(str)
    #define LCD_Backlight(state)    I2C_LCD_Backlight(state)
    #define LCD_Display(state)      I2C_LCD_Display(state)
    
#elif (LCD_DRIVER_TYPE == LCD_USE_PARALLEL)
    #include "char_lcd.h"
    
    #define LCD_Init()              LCD_init()
    #define LCD_Clear()             LCD_clear()
    #define LCD_Home()              CharLcd_Home()
    #define LCD_SetCursor(r, c)     LCD_goto_xy(r, c)
    #define LCD_WriteChar(ch)       LCD_send_char(ch)
    #define LCD_WriteString(str)    LCD_send_string(str)
    #define LCD_Backlight(state)    
    #define LCD_Display(state)      
    
#else
    #error "Invalid LCD_DRIVER_TYPE selected"
#endif

#endif
