/**
 * @file Timer.h
 * @brief Timer driver header
 */

#ifndef TIMER_H
#define TIMER_H

#include "Std_Types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer_types.h"
#include "Timer_cfg.h"

/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
void Timer_Init(Timer_cfg_t* strTimer_cfg);

 /**
  * Input: 
  * 	Timer_CH_NO: The channel number of the timer needed to be started.
  *	Timer_Count: The start value of the timer.
  * Output:
  * In/Out:			
  * Return: The error status of the function.			
  * Description: This function strats the needed timer.
  * 							
  */
void Timer_Start(TimerChan_t enuTimerCH, uint16_t u16Timer_Count);
 
 /**
  * Input: 
  * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
  * Output:
  * In/Out:			
  * Return: The error status of the function.			
  * Description: This function stops the needed timer.
  * 							
  */
void Timer_Stop(TimerChan_t enuTimerCH);
 
 /**
  * Input: 
  * 	Timer_CH_NO: The channel number of the timer needed to get its status.
  * Output:
  * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
  * In/Out:			
  * Return: The error status of the function.			
  * Description: This function is used to return if the flag of the timer is raised or not.
  * 							
  */
//void Timer_GetStatus(TimerChan_t enuTimerCH, uint8_t* u8Data);
 
 /**
  * Input: 
  * 	Timer_CH_NO: The channel number of the timer needed to get its value.
  * Output:
  * 	Data: This is the output variable of the function which holds the value of the timer.
  * In/Out:			
  * Return: The error status of the function.			
  * Description: This function is used to return the value of the timer.
  * 							
  */
uint16_t Timer_GetValue(TimerChan_t enuTimerCH);

inline uint16_t Timer_GetPrescalerValue(TimerPre_t enuTimerPre);
 

#endif /* DIO_H */
