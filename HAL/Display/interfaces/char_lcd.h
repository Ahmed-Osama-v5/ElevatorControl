//*****************************************************************************
//
// File Name	: 'lcd_lib.h'
// Title		: 8-bit LCd interface
// Author		: Ahmed Osama
// Date			: Mar 5 2019
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
#ifndef CHAR_LCD_H_
#define CHAR_LCD_H_
#ifndef F_CPU
 #define F_CPU	8000000UL
 #endif
#include "char_lcd_gcfg.h"
#include "dio.h"

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

/* LCD Commands definition */
#define Lcd_clear           	(0x01)
#define Return_home         	(0x02)
#define Decrement_cursor    	(0x04)
#define Shift_display_right 	(0x05)
#define Increment_cursor    	(0x06)
#define Shift_display_left  	(0x07)
#define Shift_cursor_left   	(0x10)
#define Shift_cursor_right  	(0x14)
#define Four_bit            	(0x28)
#define Eight_bit           	(0x38)
#define Cursor_off          	(0x0C)
#define Cursor_on           	(0x0E)

// cursor position to DDRAM mapping
#define LCD_DDRAM           	(7)		//DB7: set DD RAM address
#define LCD_LINE0_DDRAMADDR		(0x00)
#define LCD_LINE1_DDRAMADDR		(0x40)
#define LCD_LINE2_DDRAMADDR		(0x14)
#define LCD_LINE3_DDRAMADDR		(0x54)

#define LINE_0					(0)
#define LINE_1					(1)
#define LINE_2					(2)
#define LINE_3					(3)


void LCD_EN_pulse(void);
void LCD_send_char(char data);		//forms data ready to send to LCD
void LCD_send_command(uint8_t data);	//forms command ready to send to LCD
void LCD_init(void);			//Initializes LCD
void LCD_clear(void);				//Clears LCD
void LCD_send_string(const char* data);	//Outputs string to LCD
void LCD_goto_xy(uint8_t Col, uint8_t Line);	//Cursor to X Y position
void LCD_send_int(uint16_t var, uint8_t numOfDigits);


#endif // LCD_H_

