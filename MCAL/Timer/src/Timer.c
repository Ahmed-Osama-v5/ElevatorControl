/*
 * dio.c
 *
 *  Created on: Dec 22, 2019
 *      Author: Ahmed
 */
 #include "Timer.h"


 static uint8_t gsau8_preScaler[MAX_TIMER_CHANNELS] = {TIMER_NO_CLOCK, TIMER_NO_CLOCK, TIMER_NO_CLOCK};

/**
 * Input: Pointer to callback function.
 * Output:
 * In/Out:
 * Return: void
 * Description: Calls the callback function.
 */
 static void (*Timer_OVF_CBK_Ptr[MAX_TIMER_CHANNELS])(void) = {NULL};


 /**
  * Input: Pointer to a structure contains the information needed to initialize the timer.
  * Output:
  * In/Out:
  * Return: The error status of the function.
  * Description: Initiates the module.
  */
void Timer_Init(Timer_cfg_t* strTimer_cfg)
{
    if(NULL != strTimer_cfg)
    {
        gsau8_preScaler[strTimer_cfg->enuTimerCH] = strTimer_cfg->enuTimerPre;
        /* Pass pointer to callback function */
        if(NULL != strTimer_cfg->CBK_Ptr)
        {
            Timer_OVF_CBK_Ptr[strTimer_cfg->enuTimerCH] = strTimer_cfg->CBK_Ptr;
        }
        else
        {
            /* Do nothing */
        }
        switch(strTimer_cfg->enuTimerCH)
        {
        case (TIMER_CH0):
            /* Normal mode */
            TCCR0 &= ~((1 << WGM01)) | (1 << WGM00);

            if(strTimer_cfg->enumTimerIntMode == TIMER_INT_ENABLED)
            {
                TIMSK |= (1 << TOIE0);
            }
            else
            {
                TIMSK &= ~(1 << TOIE0);
            }
            break;
        case (TIMER_CH1):
            /* Normal mode */
            TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
            TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
            if(strTimer_cfg->enumTimerIntMode == TIMER_INT_DISABLED)
            {
                TIMSK &= ~(1 << TOIE1);
            }
            else
            {
                TIMSK |= (1 << TOIE1);
            }
            break;
        case (TIMER_CH2):
            /* Normal mode */
            TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
            if(strTimer_cfg->enumTimerIntMode == TIMER_INT_DISABLED)
            {
                TIMSK &= ~(1 << TOIE2);
            }
            else
            {
                TIMSK |= (1 << TOIE2);
            }
            break;
        default:
            /* Do nothing */
            break;
        }
    }
    else
    {
        /* Do nothing */
    }
 }
 
 /**
  * Input:
  * 	Timer_CH_NO: The channel number of the timer needed to be started.
  *	Timer_Count: The start value of the timer.
  * Output:
  * In/Out:
  * Return: The error status of the function.
  * Description: This function starts the needed timer.
  *
  */
void Timer_Start(TimerChan_t enuTimerCH, uint16_t u16Timer_Count)
{
    switch(enuTimerCH)
    {
    case(TIMER_CH0):
        //TCNT0 = (0xFF - (uint8)Timer_Count); /* pre-load timer */
        TCNT0 = (uint8_t)u16Timer_Count; /* pre-load timer */
        /* set prescaler */
        switch (gsau8_preScaler[enuTimerCH])
        {
        case (TIMER_PRESCALER_NO):
            /* 1 */
            TCCR0 &= ~((1 << CS02) | (1 << CS01));
            TCCR0 |= (1 << CS00);
            break;
        case (TIMER_PRESCALER_8):
            /* 2 */
            TCCR0 &= ~((1 << CS02) | (1 << CS00));
            TCCR0 |= (1 << CS01);
            break;
        case (TIMER_PRESCALER_64):
            /* 3 */
            TCCR0 &= ~(1 << CS02);
            TCCR0 |= ((1 << CS01) | (1 << CS00));
            break;
        case (TIMER_PRESCALER_256):
            /* 4 */
            TCCR0 &= ~((1 << CS01) | (1 << CS00));
            TCCR0 |= (1 << CS02);
            break;
        case (TIMER_PRESCALER_1024):
            /* 5 */
            TCCR0 &= ~(1 << CS01);
            TCCR0 |= ((1 << CS02) | (1 << CS00));
            break;
        default:
            /* Do nothing */
            break;
        }
        break;
    case(TIMER_CH1):
        TCNT1 = (0xFFFF - u16Timer_Count); /* pre-load timer */
        /* set prescaler */
        switch (gsau8_preScaler[enuTimerCH])
        {
        case (TIMER_PRESCALER_NO):
            /* 1 */
            TCCR1B &= ~((1 << CS12) | (1 << CS11));
            TCCR1B |= (1 << CS10);
            break;
        case (TIMER_PRESCALER_8):
            /* 2 */
            TCCR1B &= ~((1 << CS12) | (1 << CS10));
            TCCR1B |= (1 << CS11);
            break;
        case (TIMER_PRESCALER_64):
            /* 3 */
            TCCR1B &= ~(1 << CS12);
            TCCR1B |= ((1 << CS11) | (1 << CS10));
            break;
        case (TIMER_PRESCALER_256):
            /* 4 */
            TCCR1B &= ~((1 << CS11) | (1 << CS10));
            TCCR1B |= (1 << CS12);
            break;
        case (TIMER_PRESCALER_1024):
            /* 5 */
            TCCR1B &= ~(1 << CS11);
            TCCR1B |= ((1 << CS12) | (1 << CS10));
            break;
            default:
            /* Do nothing */
                break;
        }
        break;
    case(TIMER_CH2):
        TCNT2 = (0xFF - (uint8_t)u16Timer_Count); /* pre-load timer */
        /* set prescaler */
        switch (gsau8_preScaler[enuTimerCH])
        {
        case (TIMER_PRESCALER_NO):
            /* 1 */
            TCCR2 &= ~((1 << CS22) | (1 << CS21));
            TCCR2 |= (1 << CS20);
            break;
        case (TIMER_PRESCALER_8):
            /* 2 */
            TCCR2 &= ~((1 << CS22) | (1 << CS20));
            TCCR2 |= (1 << CS21);
            break;
        case (TIMER_PRESCALER_32):
            /* 3 */
            TCCR2 &= ~(1 << CS22);
            TCCR2 |= ((1 << CS21) | (1 << CS20));
            break;
        case (TIMER_PRESCALER_64):
            /* 4 */
            TCCR2 &= ~((1 << CS21) | (1 << CS20));
            TCCR2 |= (1 << CS22);
            break;
        case (TIMER_PRESCALER_128):
            /* 5 */
            TCCR2 &= ~(1 << CS21);
            TCCR2 |= ((1 << CS22) | (1 << CS20));
            break;
        case (TIMER_PRESCALER_256):
            /* 6 */
            TCCR2 &= ~(1 << CS20);
            TCCR2 |= ((1 << CS22) | (1 << CS21));
            break;
        case (TIMER_PRESCALER_1024):
            /* 7 */
            TCCR2 |= ((1 << CS22) | (1 << CS21) | (1 << CS20));
            break;
        default:
            /* Do nothing */
            break;
        }
        break;
    default:
        /* Do nothing */
        break;
    }
}
 
 /**
  * Input:
  * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
  * Output:
  * In/Out:
  * Return: The error status of the function.
  * Description: This function stops the needed timer.
  *
  */
void Timer_Stop(TimerChan_t enuTimerCH)
{
    switch(enuTimerCH)
    {
    case(TIMER_CH0):
        TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
        break;
    case(TIMER_CH1):
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
        break;
    case(TIMER_CH2):
        TCCR2 &= ~((1 << CS22) | (1 << CS21) | (1 << CS20));
        break;
    default:
        /* Do nothing */
        break;
    }
}


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
  /*
 ERROR_STATUS Timer_GetStatus(uint8_t Timer_CH_NO, uint8_t* Data)
 {
     if(gsau8_initState[Timer_CH_NO] == INITIALIZED)
     {
         *Data = 0;
         switch (Timer_CH_NO) {
         case (TIMER_CH0):
             if(TIFR & (1 << TOV0))
             {
                 *Data = 1;
             }
             else
             {
                 *Data = 0;
             }
             break;
         case (TIMER_CH1):
             if(TIFR & (1 << TOV1))
             {
                 *Data = 1;
             }
             else
             {
                 *Data = 0;
             }
             break;
         case (TIMER_CH2):
             if(TIFR & (1 << TOV2))
             {
                 *Data = 1;
             }
             else
             {
                 *Data = 0;
             }
             break;
         default:
             return E_NOK;
             break;
         }
     }
     else
     {
         return E_NOK;
     }
     return E_OK;
 }
 */
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
uint16_t Timer_GetValue(TimerChan_t enuTimerCH)
{
    uint16_t u16retVal = 0;
    uint16_t timerCount;
    uint32_t ticksPerMs = (F_CPU / Timer_GetPrescalerValue(TIMER1_PRESCALER)) / 1000;
    uint16_t milliseconds;

    switch (enuTimerCH)
    {
    case (TIMER_CH0):
        timerCount = TCNT0;
        milliseconds = timerCount / ticksPerMs;
        u16retVal = milliseconds;
        break;
    case (TIMER_CH1):
        timerCount = TCNT1;
        milliseconds = timerCount / ticksPerMs;
        u16retVal = milliseconds;
        //u16retVal = TCNT1;
        break;
    case (TIMER_CH2):
        timerCount = TCNT2;
        milliseconds = timerCount / ticksPerMs;
        u16retVal = milliseconds;
        break;
    default:
        timerCount = TCNT1;
        milliseconds = timerCount / ticksPerMs;
        u16retVal = milliseconds;
        break;
    }
    return u16retVal;
 }

 inline uint16_t Timer_GetPrescalerValue(TimerPre_t enuTimerPre)
 {
    switch (enuTimerPre)
    {
    case TIMER_PRESCALER_NO:
        return (uint16_t) 1;
    case TIMER_PRESCALER_8:
        return (uint16_t) 8;
    case TIMER_PRESCALER_32:
        return (uint16_t) 32;
    case TIMER_PRESCALER_64:
        return (uint16_t) 64;
    case TIMER_PRESCALER_128:
        return (uint16_t) 128;
    case TIMER_PRESCALER_256:
        return (uint16_t) 256;
    case TIMER_PRESCALER_1024:
        return (uint16_t) 1024;
    default:
        return (uint16_t) 1;
    }
 }
 
 ISR(TIMER0_OVF_vect)
 {
     Timer_OVF_CBK_Ptr[0]();
 }
 
 ISR(TIMER1_OVF_vect)
 {
     Timer_OVF_CBK_Ptr[1]();
 }
 
 ISR(TIMER2_OVF_vect)
 {
     Timer_OVF_CBK_Ptr[2]();
 }
 