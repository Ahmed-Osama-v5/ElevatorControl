/*
 * dio.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Ahmed
 */
 #include "dio.h"

/**
 * @brief Initialize the DIO driver
 * @param Config Pointer to configuration structure
 */
 void DIO_Init(const Dio_Cfg_t* Config)
 {
     if(1)
     {
         switch (Config->enuGPIO)
         {
         case (GPIOA):
             if(Config->enuPinDir == DIR_OUTPUT)
             {
                 DDRA |= (1 << Config->u8Pin);
             }
             else
             {
                 DDRA &= ~(1 << Config->u8Pin);
             }
             break;
         case (GPIOB):
             if(Config->enuPinDir == DIR_OUTPUT)
             {
                 DDRB |= (1 << Config->u8Pin);
             }
             else
             {
                 DDRB &= ~(1 << Config->u8Pin);
             }
             break;
         case (GPIOC):
             if(Config->enuPinDir == DIR_OUTPUT)
             {
                 DDRC |= (1 << Config->u8Pin);
             }
             else
             {
                 DDRC &= ~(1 << Config->u8Pin);
             }
             break;
         case (GPIOD):
             if(Config->enuPinDir == DIR_OUTPUT)
             {
                 DDRD |= (1 << Config->u8Pin);
             }
             else
             {
                 DDRD &= ~(1 << Config->u8Pin);
             }
             break;
         default:
             break;
         }
     }
     else
     {
         /* Do nothing */
     }
 }
 
 
 /**
 * @brief Write a value to a pin
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param u8Pin Pin number (0-7)
 * @param enuPinState Pin State (STATE_LOW or STATE_HIGH)
 */
void DIO_WritePin(GPIO_t port, uint8_t u8Pin, PinState_t enuPinState)
 {
     switch(port)
     {
     case (GPIOA):
         if(enuPinState == STATE_HIGH)
         {
             PORTA |= (1 << u8Pin);
         }
         else
         {
             PORTA &= ~(1 << u8Pin);
         }
         break;
     case (GPIOB):
         if(enuPinState == STATE_HIGH)
         {
             PORTB |= (1 << u8Pin);
         }
         else
         {
             PORTB &= ~(1 << u8Pin);
         }
         break;
     case (GPIOC):
         if(enuPinState == STATE_HIGH)
         {
             PORTC |= (1 << u8Pin);
         }
         else
         {
             PORTC &= ~(1 << u8Pin);
         }
         break;
     case (GPIOD):
         if(enuPinState == STATE_HIGH)
         {
             PORTD |= (1 << u8Pin);
         }
         else
         {
             PORTD &= ~(1 << u8Pin);
         }
         break;
     default:
         break;
     }
 }
 
 /**
 * @brief Read a pin value
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param u8Pin Pin number (0-7)
 * @return Pin State (STATE_LOW or STATE_HIGH)
 */
PinState_t DIO_ReadPin(GPIO_t port, uint8_t u8Pin)
 {
    PinState_t retState = STATE_LOW; // Default state
    switch(port)
    {
    case (GPIOA):
        if(PINA & (1 << u8Pin))
        {
            retState = STATE_HIGH;
        }
        else
        {
            retState = STATE_LOW;
        }
        break;
    case (GPIOB):
        if(PINB & (1 << u8Pin))
        {
            retState = STATE_HIGH;
        }
        else
        {
            retState = STATE_LOW;
        }
        break;
    case (GPIOC):
        if(PINC & (1 << u8Pin))
        {
            retState = STATE_HIGH;
        }
        else
        {
            retState = STATE_LOW;
        }
        break;
    case (GPIOD):
        if(PIND & (1 << u8Pin))
        {
            retState = STATE_HIGH;
        }
        else
        {
            retState = STATE_LOW;
        }
        break;
    default:
        break;
    }
    return retState;
 }

 /**
 * @brief Write a value to an entire port
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @param value Port value (8-bit)
 */
void DIO_WritePort(GPIO_t port, uint8_t value)
{
    switch(port)
    {
    case (GPIOA):
        PORTA = value;
        break;
    case (GPIOB):
        PORTB = value;
        break;
    case (GPIOC):
        PORTC = value;
        break;
    case (GPIOD):
        PORTD = value;
        break;
    default:
        break;
    }
}

/**
 * @brief Read an entire port value
 * @param port Port ID (GPIOA, GPIOB, GPIOC, GPIOD)
 * @return Port value (8-bit)
 */
 uint8_t DIO_ReadPort(GPIO_t port)
 {
     uint8_t portValue = 0; // Default value
     switch(port)
     {
     case (GPIOA):
         portValue = PINA;
         break;
     case (GPIOB):
         portValue = PINB;
         break;
     case (GPIOC):
         portValue = PINC;
         break;
     case (GPIOD):
         portValue = PIND;
         break;
     default:
         break;
     }
     return portValue;
 }
 
 /**
  * @brief Toggles the state of a pin
  * 
  * @param GPIO 
  * @param pins 
  */
 void DIO_Toggle (GPIO_t port, uint8_t u8Pin)
 {
    switch(port)
    {
    case (GPIOA):
        PORTA ^= (1 << u8Pin);
        break;
    case (GPIOB):
        PORTB ^= (1 << u8Pin);
        break;
    case (GPIOC):
        PORTC ^= (1 << u8Pin);
        break;
    case (GPIOD):
        PORTD ^= (1 << u8Pin);
        break;
    default:
        break;
    }
 }