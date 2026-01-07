#ifndef CHAR_LCD_GCFG_H
#define CHAR_LCD_GCFG_H

#define LCD_FOUR_BIT    1
#define LCD_EIGHT_BIT   2

#define LCD_MODE        LCD_FOUR_BIT

#ifndef LCD_RS_PIN
#define LCD_RS_PIN	0 	//define MCU pin connected to LCD RS
#endif
#ifndef LCD_RS_GPIO
#define LCD_RS_GPIO	GPIOB
#endif

#ifndef LCD_EN_PIN
#define LCD_EN_PIN	1	//define MCU pin connected to LCD E
#endif
#ifndef LCD_EN_GPIO
#define LCD_EN_GPIO	GPIOB
#endif

#define LCD_DATA_DDR	DDRA
#define LCD_DATA_PORT	GPIOA

#define LCD_D4_GPIO    GPIOA
#define LCD_D4_PIN     4

#define LCD_D5_GPIO    GPIOA
#define LCD_D5_PIN     5

#define LCD_D6_GPIO    GPIOA
#define LCD_D6_PIN     6

#define LCD_D7_GPIO    GPIOA
#define LCD_D7_PIN     7

#endif // CHAR_LCD_GCFG_H
