/**
 * @file dio.c
 * @brief Digital Input/Output driver implementation
 */

#include "dio.h"
//#include "dio_cfg.h"

/* Static helper functions */
static volatile uint8_t* DIO_GetDirectionRegister(DIO_PortType port);
static volatile uint8_t* DIO_GetPortRegister(DIO_PortType port);
static volatile uint8_t* DIO_GetPinRegister(DIO_PortType port);

void DIO_Init(const DIO_ConfigType* Config)
{
    uint8_t pin;

    /* Configure PORTA */
    for (pin = 0; pin < 8; pin++) {
        if (Config->portA.pins[pin].direction == DIO_PIN_OUTPUT) {
            DDRA |= (1 << pin);
        } else {
            DDRA &= ~(1 << pin);
            if (Config->portA.pins[pin].pullUp) {
                PORTA |= (1 << pin);
            }
        }
    }

    /* Configure PORTB */
    for (pin = 0; pin < 8; pin++) {
        if (Config->portB.pins[pin].direction == DIO_PIN_OUTPUT) {
            DDRB |= (1 << pin);
        } else {
            DDRB &= ~(1 << pin);
            if (Config->portB.pins[pin].pullUp) {
                PORTB |= (1 << pin);
            }
        }
    }

    /* Configure PORTC */
    for (pin = 0; pin < 8; pin++) {
        if (Config->portC.pins[pin].direction == DIO_PIN_OUTPUT) {
            DDRC |= (1 << pin);
        } else {
            DDRC &= ~(1 << pin);
            if (Config->portC.pins[pin].pullUp) {
                PORTC |= (1 << pin);
            }
        }
    }

    /* Configure PORTD */
    for (pin = 0; pin < 8; pin++) {
        if (Config->portD.pins[pin].direction == DIO_PIN_OUTPUT) {
            DDRD |= (1 << pin);
        } else {
            DDRD &= ~(1 << pin);
            if (Config->portD.pins[pin].pullUp) {
                PORTD |= (1 << pin);
            }
        }
    }
}

void DIO_WritePin(DIO_PortType port, DIO_PinType pin, DIO_LevelType value)
{
    volatile uint8_t* portReg = DIO_GetPortRegister(port);
    if (portReg != NULL && pin < 8) {
        if (value == DIO_PIN_HIGH) {
            *portReg |= (1 << pin);
        } else {
            *portReg &= ~(1 << pin);
        }
    }
}

DIO_LevelType DIO_ReadPin(DIO_PortType port, DIO_PinType pin)
{
    volatile uint8_t* pinReg = DIO_GetPinRegister(port);
    if (pinReg != NULL && pin < 8) {
        return (*pinReg & (1 << pin)) ? DIO_PIN_HIGH : DIO_PIN_LOW;
    }
    return DIO_PIN_LOW;
}

void DIO_WritePort(DIO_PortType port, uint8_t value)
{
    volatile uint8_t* portReg = DIO_GetPortRegister(port);
    if (portReg != NULL) {
        *portReg = value;
    }
}

uint8_t DIO_ReadPort(DIO_PortType port)
{
    volatile uint8_t* pinReg = DIO_GetPinRegister(port);
    if (pinReg != NULL) {
        return *pinReg;
    }
    return 0;
}

/* Helper function implementations */
static volatile uint8_t* DIO_GetDirectionRegister(DIO_PortType port)
{
    switch (port) {
    case DIO_PORTA: return &DDRA;
    case DIO_PORTB: return &DDRB;
    case DIO_PORTC: return &DDRC;
    case DIO_PORTD: return &DDRD;
    default: return NULL;
    }
}

static volatile uint8_t* DIO_GetPortRegister(DIO_PortType port)
{
    switch (port) {
    case DIO_PORTA: return &PORTA;
    case DIO_PORTB: return &PORTB;
    case DIO_PORTC: return &PORTC;
    case DIO_PORTD: return &PORTD;
    default: return NULL;
    }
}

static volatile uint8_t* DIO_GetPinRegister(DIO_PortType port)
{
    switch (port) {
    case DIO_PORTA: return &PINA;
    case DIO_PORTB: return &PINB;
    case DIO_PORTC: return &PINC;
    case DIO_PORTD: return &PIND;
    default: return NULL;
    }
}
