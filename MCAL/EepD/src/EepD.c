
#define EepD_c

#include <avr/io.h>
#include "EepD_lcfg.h"
#include "EepD.h"

uint8_t EEPROM_u8Read(const uint8_t u8Address, uint8_t *pu8Data)
{
    if(u8Address > cu8EEPROM_SIZE){
		return cu8EEPROM_INVALID_ADDR;
	}
	
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEARH = (u8Address & 0xFF00) >> 8;
	EEARL = (u8Address & 0x00FF);
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	*pu8Data = EEDR;
	return cu8EEPROM_OK;
}

uint8_t EEPROM_u8Write(const uint8_t u8Address, const uint8_t u8Data)
{
	if(u8Address > cu8EEPROM_SIZE){
		return cu8EEPROM_INVALID_ADDR;
	}
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and Data Registers */
	EEARH = (u8Address & 0xFF00) >> 8;
	EEARL = (u8Address & 0x00FF);
	EEDR = u8Data;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEWE);
	return cu8EEPROM_OK;
}

uint8_t EEPROM_u8Update(const uint8_t u8Address, const uint8_t u8Data)
{
    uint8_t u8ErrRetVal = cu8EEPROM_OK;
	if(u8Address > cu8EEPROM_SIZE){
		return cu8EEPROM_INVALID_ADDR;
	}
	uint8_t value = 0;

	u8ErrRetVal = EEPROM_u8Read(u8Address,&value);
	if(u8ErrRetVal != cu8EEPROM_OK){
		return u8ErrRetVal;
	}
	if(value == u8Data){
		return cu8EEPROM_OK;
	}

	u8ErrRetVal = EEPROM_u8Write(u8Address,u8Data);
	if(u8ErrRetVal != cu8EEPROM_OK){
		return u8ErrRetVal;
	}

	u8ErrRetVal = EEPROM_u8Read(u8Address,&value);
	if(u8ErrRetVal != cu8EEPROM_OK){
		return u8ErrRetVal;
	}
	if(value != u8Data){
		return cu8EEPROM_WRITE_FAIL;
	}

	return cu8EEPROM_OK;
}