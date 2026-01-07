#include "soft_i2c.h"

static void SoftI2C_SDA_High(void);
static void SoftI2C_SDA_Low(void);
static void SoftI2C_SCL_High(void);
static void SoftI2C_SCL_Low(void);
static uint8_t SoftI2C_SDA_Read(void);
static void SoftI2C_Delay(void);

static void SoftI2C_SDA_High(void) {
    DIO_WritePin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, STATE_HIGH);
}

static void SoftI2C_SDA_Low(void) {
    DIO_WritePin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN, STATE_LOW);
}

static void SoftI2C_SCL_High(void) {
    DIO_WritePin(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN, STATE_HIGH);
}

static void SoftI2C_SCL_Low(void) {
    DIO_WritePin(SOFT_I2C_SCL_PORT, SOFT_I2C_SCL_PIN, STATE_LOW);
}

static uint8_t SoftI2C_SDA_Read(void) {
    return (DIO_ReadPin(SOFT_I2C_SDA_PORT, SOFT_I2C_SDA_PIN) == STATE_HIGH) ? 1 : 0;
}

static void SoftI2C_Delay(void) {
    _delay_us(SOFT_I2C_DELAY_US);
}

void SoftI2C_Init(void) {
    SoftI2C_SDA_High();
    SoftI2C_SCL_High();
    SoftI2C_Delay();
}

void SoftI2C_Start(void) {
    SoftI2C_SDA_High();
    SoftI2C_SCL_High();
    SoftI2C_Delay();
    SoftI2C_SDA_Low();
    SoftI2C_Delay();
    SoftI2C_SCL_Low();
    SoftI2C_Delay();
}

void SoftI2C_Stop(void) {
    SoftI2C_SDA_Low();
    SoftI2C_Delay();
    SoftI2C_SCL_High();
    SoftI2C_Delay();
    SoftI2C_SDA_High();
    SoftI2C_Delay();
}

uint8_t SoftI2C_WriteByte(uint8_t data) {
    uint8_t i;
    uint8_t ack;
    
    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            SoftI2C_SDA_High();
        } else {
            SoftI2C_SDA_Low();
        }
        SoftI2C_Delay();
        SoftI2C_SCL_High();
        SoftI2C_Delay();
        SoftI2C_SCL_Low();
        SoftI2C_Delay();
        data <<= 1;
    }
    
    SoftI2C_SDA_High();
    SoftI2C_Delay();
    SoftI2C_SCL_High();
    SoftI2C_Delay();
    ack = SoftI2C_SDA_Read();
    SoftI2C_SCL_Low();
    SoftI2C_Delay();
    
    return ack;
}

uint8_t SoftI2C_ReadByte(uint8_t ack) {
    uint8_t i;
    uint8_t data = 0;
    
    SoftI2C_SDA_High();
    
    for (i = 0; i < 8; i++) {
        data <<= 1;
        SoftI2C_SCL_High();
        SoftI2C_Delay();
        if (SoftI2C_SDA_Read()) {
            data |= 0x01;
        }
        SoftI2C_SCL_Low();
        SoftI2C_Delay();
    }
    
    if (ack == SOFT_I2C_ACK) {
        SoftI2C_SDA_Low();
    } else {
        SoftI2C_SDA_High();
    }
    SoftI2C_Delay();
    SoftI2C_SCL_High();
    SoftI2C_Delay();
    SoftI2C_SCL_Low();
    SoftI2C_Delay();
    SoftI2C_SDA_High();
    
    return data;
}

uint8_t SoftI2C_Write(uint8_t address, uint8_t* data, uint8_t length) {
    uint8_t i;
    
    SoftI2C_Start();
    SoftI2C_WriteByte(address << 1);
    /*
    if (SoftI2C_WriteByte(address << 1) != SOFT_I2C_ACK) {
        SoftI2C_Stop();
        return 0;
    }*/
    
    for (i = 0; i < length; i++) {
        SoftI2C_WriteByte(data[i]);
        /*
        if (SoftI2C_WriteByte(data[i]) != SOFT_I2C_ACK) {
            SoftI2C_Stop();
            return 0;
        }*/
    }
    
    SoftI2C_Stop();
    return 1;
}

uint8_t SoftI2C_Read(uint8_t address, uint8_t* data, uint8_t length) {
    uint8_t i;
    
    SoftI2C_Start();
    
    if (SoftI2C_WriteByte((address << 1) | 0x01) != SOFT_I2C_ACK) {
        SoftI2C_Stop();
        return 0;
    }
    
    for (i = 0; i < length; i++) {
        if (i == length - 1) {
            data[i] = SoftI2C_ReadByte(SOFT_I2C_NACK);
        } else {
            data[i] = SoftI2C_ReadByte(SOFT_I2C_ACK);
        }
    }
    
    SoftI2C_Stop();
    return 1;
}
