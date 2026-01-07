# LCD Driver Configuration

This project supports two LCD driver types:
1. **Parallel LCD Driver** - Direct GPIO connection (existing driver)
2. **I2C LCD Driver** - Software I2C with PCF8574 I2C expander

## Hardware Configuration

### I2C LCD Connections
- **PB0** - SDA (I2C Data)
- **PB1** - SCL (I2C Clock)
- **I2C Address** - 0x27 (default, can be changed in `i2c_lcd.h`)

### Parallel LCD Connections
- Uses existing GPIO pins as configured in `char_lcd_lcfg.h`

## Switching Between Drivers

To switch between LCD drivers, modify the `LCD_DRIVER_TYPE` in `HAL/Display/interfaces/lcd_config.h`:

```c
#define LCD_DRIVER_TYPE  LCD_USE_PARALLEL    // Use parallel LCD
// OR
#define LCD_DRIVER_TYPE  LCD_USE_I2C         // Use I2C LCD
```

## Usage Example

```c
#include "lcd_config.h"

int _main___(void) {
    LCD_Init();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_WriteString("Hello World!");
    LCD_SetCursor(1, 0);
    LCD_WriteString("I2C LCD Driver");
    
    #if (LCD_DRIVER_TYPE == LCD_USE_I2C)
    LCD_Backlight(1);  // Turn on backlight (I2C only)
    #endif
    
    while(1) {
        // Your code here
    }
    
    return 0;
}
```

## API Functions

Both drivers support the following common functions:
- `LCD_Init()` - Initialize the LCD
- `LCD_Clear()` - Clear the display
- `LCD_Home()` - Move cursor to home position
- `LCD_SetCursor(row, col)` - Set cursor position
- `LCD_WriteChar(ch)` - Write a single character
- `LCD_WriteString(str)` - Write a string

### I2C LCD Additional Functions
- `LCD_Backlight(state)` - Control backlight (1=ON, 0=OFF)
- `LCD_Display(state)` - Control display (1=ON, 0=OFF)

## Software I2C Configuration

The software I2C driver can be configured in `HAL/SoftI2C/interfaces/soft_i2c.h`:

```c
#define SOFT_I2C_SDA_PORT    GPIOB    // SDA port
#define SOFT_I2C_SDA_PIN     0        // SDA pin
#define SOFT_I2C_SCL_PORT    GPIOB    // SCL port
#define SOFT_I2C_SCL_PIN     1        // SCL pin
#define SOFT_I2C_DELAY_US    5        // I2C timing delay in microseconds
```

## Files Structure

```
HAL/
├── SoftI2C/
│   ├── interfaces/
│   │   └── soft_i2c.h          # Software I2C header
│   └── src/
│       └── soft_i2c.c          # Software I2C implementation
├── I2C_LCD/
│   ├── interfaces/
│   │   └── i2c_lcd.h           # I2C LCD driver header
│   └── src/
│       └── i2c_lcd.c           # I2C LCD driver implementation
└── Display/
    ├── interfaces/
    │   ├── lcd_config.h        # LCD driver selection
    │   └── char_lcd.h          # Parallel LCD header
    └── src/
        └── char_lcd.c          # Parallel LCD implementation
```

## Notes

1. Make sure to configure PB0 and PB1 as outputs in your DIO configuration when using I2C LCD
2. The I2C LCD driver assumes a PCF8574-based I2C backpack with standard pinout
3. The default I2C address is 0x27, but some modules use 0x3F - adjust in `i2c_lcd.h` if needed
4. The software I2C timing can be adjusted via `SOFT_I2C_DELAY_US` for different clock speeds
