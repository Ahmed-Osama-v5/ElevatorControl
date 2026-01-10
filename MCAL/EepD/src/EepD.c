
#define EepD_c

#include <avr/io.h>
#include "EepD_lcfg.h"
#include "EepD.h"
#include <avr/eeprom.h>

uint8_t EEPROM_u8Read(uint8_t u8Address, uint8_t *pu8Data)
{
    if(u8Address > cu8EEPROM_SIZE){
		return cu8EEPROM_INVALID_ADDR;
	}
	*pu8Data = eeprom_read_byte((uint8_t*)u8Address);
	return cu8EEPROM_OK;
}

uint8_t EEPROM_u8Write(uint8_t u8Address, const uint8_t u8Data)
{
	if(u8Address > cu8EEPROM_SIZE){
		return cu8EEPROM_INVALID_ADDR;
	}
	eeprom_write_byte((uint8_t*)u8Address, u8Data);
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