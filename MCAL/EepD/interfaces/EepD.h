#ifndef EEPD_H
#define EEPD_H

#include "Std_Types.h"
#include "SystemConfig.h"
#include "EepD_gcfg.h"

/* Make sure there are no circular dependencies */
#ifdef __cplusplus
extern "C" {
#endif


uint8_t EEPROM_u8Read(const uint8_t u8Address, uint8_t *pu8Data);
uint8_t EEPROM_u8Write(const uint8_t u8Address, const uint8_t u8Data);
uint8_t EEPROM_u8Update(const uint8_t u8Address, const uint8_t u8Data);

#ifdef __cplusplus
}
#endif

#endif /* EEPD_H */