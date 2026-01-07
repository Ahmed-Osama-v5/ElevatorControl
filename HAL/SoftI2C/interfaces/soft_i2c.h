#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include <stdint.h>
#include <util/delay.h>
#include "dio.h"

#define SOFT_I2C_SDA_PORT    GPIOB
#define SOFT_I2C_SDA_PIN     0
#define SOFT_I2C_SCL_PORT    GPIOB
#define SOFT_I2C_SCL_PIN     1

#define SOFT_I2C_DELAY_US    5

#define SOFT_I2C_ACK         0
#define SOFT_I2C_NACK        1

void SoftI2C_Init(void);
void SoftI2C_Start(void);
void SoftI2C_Stop(void);
uint8_t SoftI2C_WriteByte(uint8_t data);
uint8_t SoftI2C_ReadByte(uint8_t ack);
uint8_t SoftI2C_Write(uint8_t address, uint8_t* data, uint8_t length);
uint8_t SoftI2C_Read(uint8_t address, uint8_t* data, uint8_t length);

#endif
