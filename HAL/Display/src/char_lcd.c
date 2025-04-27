//*****************************************************************************
//
// File Name	: 'lcd_lib.c'
// Title		: 8-bit LCd interface
// Author		: Ahmed Osama
// Date			: Mar 5 2019
// Target MCU	: Atmel AVR series
// Modified		: Feb 22 2023
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
#define LCD_c
#include "Std_Types.h"
#include <util/delay.h>
#include "char_lcd.h"

void LCD_EN_pulse(void)
{
	DIO_WritePin(LCD_EN_GPIO, LCD_EN_PIN, STATE_HIGH);
    _delay_us(500);
	DIO_WritePin(LCD_EN_GPIO, LCD_EN_PIN, STATE_LOW);
}

void LCD_send_char(char data)
{
	DIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, STATE_HIGH);

	DIO_WritePort(LCD_DATA_PORT, data);
	LCD_EN_pulse();
	DIO_WritePort(LCD_DATA_PORT, 0);
	_delay_ms(5);
}
void LCD_send_command(uint8_t data)
{
	DIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, STATE_LOW);
	
	DIO_WritePort(LCD_DATA_PORT, data);
	LCD_EN_pulse();
	DIO_WritePort(LCD_DATA_PORT, 0);
	_delay_ms(5);
}
void LCD_init(void)
{
    /* set LCD pins as outputs */
    Dio_Cfg_t strDio = {0};
    strDio.enuGPIO = LCD_RS_GPIO;
    strDio.enuPinDir = DIR_OUTPUT;
    strDio.u8Pin = LCD_RS_PIN;
    DIO_Init(&strDio);
    
    strDio.enuGPIO = LCD_EN_GPIO;
    strDio.u8Pin = LCD_EN_PIN;
    DIO_Init(&strDio);
	
	LCD_DATA_DDR = 0xFF;
	DIO_WritePort(LCD_DATA_PORT, 0);
	
	DIO_WritePin(LCD_RS_GPIO, LCD_RS_PIN, STATE_LOW);
	DIO_WritePin(LCD_EN_GPIO, LCD_EN_PIN, STATE_LOW);
	_delay_ms(150);

	LCD_send_command(Lcd_clear);
	_delay_ms(5);
	LCD_send_command(Eight_bit);
	_delay_us(50);
	LCD_send_command(Cursor_off);
	_delay_us(50);
}
void LCD_clear(void)
{
	LCD_send_command(Lcd_clear);
}

void LCD_send_string(const char* data)
{
	while(*data){
		LCD_send_char(*data++);
	}
}

void LCD_goto_xy(uint8_t Col, uint8_t Line)
{
	uint8_t DDRAMAddr;
	// remap lines into proper order
	switch(Line)
	{
	case LINE_0: DDRAMAddr = LCD_LINE0_DDRAMADDR+Col; break;
	case LINE_1: DDRAMAddr = LCD_LINE1_DDRAMADDR+Col; break;
	case LINE_2: DDRAMAddr = LCD_LINE2_DDRAMADDR+Col; break;
	case LINE_3: DDRAMAddr = LCD_LINE3_DDRAMADDR+Col; break;
	default: DDRAMAddr = LCD_LINE0_DDRAMADDR+Col;
	}
	// set data address
	LCD_send_command(1<<LCD_DDRAM | DDRAMAddr);

}

void LCD_send_int(uint16_t var, uint8_t numOfDigits)
{
	char str[6];
	if(numOfDigits == 2)
	{
		str[2] = '\0';
		str[1] = (var % 10) + 48;
		str[0] = ((var / 10) % 10) + 48;
	}
	else if(numOfDigits == 3)
	{
		str[3] = '\0';
		str[2] = (var % 10) + 48;
		str[1] = ((var / 10) % 10) + 48;
		str[0] = ((var / 100) % 10) + 48;
	}
	else if(numOfDigits == 4)
	{
		str[4] = '\0';
		str[3] = (var % 10) + 48;
		str[2] = ((var / 10) % 10) + 48;
		str[1] = ((var / 100) % 10) + 48;
		str[0] = ((var / 1000) % 10) + 48;
	}
	else if(numOfDigits == 5)
	{
		str[5] = '\0';
		str[4] = (var % 10) + 48;
		str[3] = ((var / 10) % 10) + 48;
		str[2] = ((var / 100) % 10) + 48;
		str[1] = ((var / 1000) % 10) + 48;
		str[0] = ((var / 10000) % 10) + 48;
	}
	else
	{
		/* Ignore */
	}
	LCD_send_string(str);
}
