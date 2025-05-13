
#define CallHandler_c

#include "Std_Types.h"

#include "CallHandler_lcfg.h"
#include "ElevatorController.h"

#include "CallHandler.h"


static CallHandler_t callHandler;

// Private function declarations
static boolean bQueue_is_empty(const CallQueue_t* cpstrQueue);
static boolean bQueue_is_full(const CallQueue_t* cpstrQueue);
static void vidQueue_enqueue(CallQueue_t* pstrQueue, uint8_t u8Floor, CallType_t enuCallType);
static ElevatorCall_t* pstrQueue_peek(CallQueue_t* pstrQueue);
static void vidQueue_dequeue(CallQueue_t* pstrQueue);
static void vidUpdate_led_state(uint8_t u8Floor, CallType_t enuCallType);
static void vidProcess_led_blink(uint8_t u8Floor);
static boolean bShould_accept_call(uint8_t u8Floor, MoveDirection_t enuDirection, CallType_t enuCallType);

void  *my_memset(void *b, int c, int len);

void CallHandler_vidInit(void)
{
    uint8_t i;

    // Initialize call queues
    (void) my_memset(&callHandler, 0, sizeof(callHandler));
    
    // Initialize LED states
    for(i = 0; i < cu8MAX_FLOORS; i++) {
        callHandler.strLedStates[i].bIsBlinking = False;
        callHandler.strLedStates[i].u16LastToggle = 0;
        callHandler.strLedStates[i].bCurrentState = False;
        callHandler.strLedStates[i].u8DutyPercent = cu8LED_INT_DUTY_CYCLE;
    }
}

void CallHandler_vidProcess(Elevator_t* pstrElevator)
{
    uint16_t u16CurrentTime = elevator_hal_u16Get_time_ms();
    
    // Process call queues based on direction
    CallQueue_t* pstrActiveQueue = (pstrElevator->enuDirection == DIR_UP) ? 
                            &callHandler.strUpCalls : &callHandler.strDownCalls;
    
    // Check if we need to change direction
    if(bQueue_is_empty(pstrActiveQueue)) {
        if(pstrElevator->enuDirection == DIR_UP && CallHandler_bHas_calls_below(pstrElevator->u8CurrentFloor)) {
            pstrElevator->enuDirection = DIR_DOWN;
            pstrActiveQueue = &callHandler.strDownCalls;
        }
        else if(pstrElevator->enuDirection == DIR_DOWN && CallHandler_bHas_calls_above(pstrElevator->u8CurrentFloor)) {
            pstrElevator->enuDirection = DIR_UP;
            pstrActiveQueue = &callHandler.strUpCalls;
        }
        else {
            pstrElevator->enuDirection = DIR_IDLE;
        }
    }
    
    // Process LEDs
    CallHandler_vidProcess_leds();
}

boolean CallHandler_bRegister_call(uint8_t u8Floor, CallType_t enuCallType, MoveDirection_t enuDirection)
{
    if(u8Floor >= cu8MAX_FLOORS) {
        return False;
    }
    
    // Don't accept calls for current floor if door is open
    if(elevator_hal_bRead_floor_sensor(u8Floor) && 
        elevator_hal_bRead_door_open_sensor()) {
        return False;
    }
    
    // Determine which queue to use
    CallQueue_t* pstrTargetQueue = (enuDirection == DIR_UP) ? 
                           &callHandler.strUpCalls : &callHandler.strDownCalls;
    
    if(!bQueue_is_full(pstrTargetQueue)) {
        vidQueue_enqueue(pstrTargetQueue, u8Floor, enuCallType);
        vidUpdate_led_state(u8Floor, enuCallType);
        return True;
    }
    
    return False;
}

void CallHandler_vidClear_call(uint8_t u8Floor)
{
    uint8_t u8I, u8J;
    // Clear calls from both queues for this floor
    CallQueue_t* pstrQueues[] = {&callHandler.strUpCalls, &callHandler.strDownCalls};
    
    for(u8I = 0; u8I < 2; u8I++) {
        CallQueue_t* pstrQueue = pstrQueues[u8I];
        uint8_t size = pstrQueue->u8Size;
        
        for(u8J = 0; u8J < size; u8J++) {
            ElevatorCall_t* pstrCall = pstrQueue_peek(pstrQueue);
            if(pstrCall != NULL)
            {
                if(pstrCall && pstrCall->u8Floor == u8Floor) {
                    vidQueue_dequeue(pstrQueue);
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    
    // Reset LED state for this floor
    callHandler.strLedStates[u8Floor].bIsBlinking = False;
    elevator_hal_vidSet_floor_led(u8Floor, False);
}

boolean CallHandler_bHas_calls_above(uint8_t u8CurrentFloor)
{
    uint8_t u8I, u8J;
    CallQueue_t* pstrQueues[] = {&callHandler.strUpCalls, &callHandler.strDownCalls};
    
    for(u8I = 0; u8I < 2; u8I++) {
        CallQueue_t* pstrQueue = pstrQueues[u8I];
        uint8_t u8Size = pstrQueue->u8Size;
        
        for(u8J = 0; u8J < u8Size; u8J++) {
            ElevatorCall_t* pstrCall = &pstrQueue->strCalls[(pstrQueue->u8Head + u8J) % cu8CALL_QUEUE_SIZE];
            if(pstrCall->u8Floor > u8CurrentFloor) {
                return True;
            }
        }
    }
    
    return False;
}

boolean CallHandler_bHas_calls_below(uint8_t u8CurrentFloor)
{
    uint8_t u8I, u8J;
    CallQueue_t* pstrQueues[] = {&callHandler.strUpCalls, &callHandler.strDownCalls};
    
    for(u8I = 0; u8I < 2; u8I++) {
        CallQueue_t* pstrQueue = pstrQueues[u8I];
        uint8_t u8Size = pstrQueue->u8Size;
        
        for(u8J = 0; u8J < u8Size; u8J++) {
            ElevatorCall_t* pstrCall = &pstrQueue->strCalls[(pstrQueue->u8Head + u8J) % cu8CALL_QUEUE_SIZE];
            if(pstrCall->u8Floor < u8CurrentFloor) {
                return True;
            }
        }
    }
    
    return False;
}

uint8_t CallHandler_u8Get_next_floor(uint8_t u8CurrentFloor, MoveDirection_t enuDirection)
{
    uint8_t u8I;
    CallQueue_t* pstrActiveQueue = (enuDirection == DIR_UP) ? 
                            &callHandler.strUpCalls : &callHandler.strDownCalls;
    
    if(bQueue_is_empty(pstrActiveQueue)) {
        return u8CurrentFloor;
    }
    
    if(enuDirection == DIR_UP) {
        uint8_t u8NextFloor = cu8MAX_FLOORS;
        for(u8I = 0; u8I < pstrActiveQueue->u8Size; u8I++) {
            ElevatorCall_t* pstrCall = &pstrActiveQueue->strCalls[(pstrActiveQueue->u8Head + u8I) % cu8CALL_QUEUE_SIZE];
            if(pstrCall->u8Floor > u8CurrentFloor && pstrCall->u8Floor < u8NextFloor) {
                u8NextFloor = pstrCall->u8Floor;
            }
        }
        return u8NextFloor;
    }
    else {
        uint8_t u8NextFloor = 0;
        for(u8I = 0; u8I < pstrActiveQueue->u8Size; u8I++) {
            ElevatorCall_t* pstrCall = &pstrActiveQueue->strCalls[(pstrActiveQueue->u8Head + u8I) % cu8CALL_QUEUE_SIZE];
            if(pstrCall->u8Floor < u8CurrentFloor && pstrCall->u8Floor > u8NextFloor) {
                u8NextFloor = pstrCall->u8Floor;
            }
        }
        return u8NextFloor;
    }
}

void CallHandler_vidProcess_leds(void)
{
    uint8_t u8Floor;
    uint16_t u16CurrentTime = elevator_hal_u16Get_time_ms();
    
    // Process LED blinking for all floors
    for(u8Floor = 0; u8Floor < cu8MAX_FLOORS; u8Floor++) {
        if(callHandler.strLedStates[u8Floor].bIsBlinking) {
            vidProcess_led_blink(u8Floor);
        }
    }
}

void  *my_memset(void *b, int c, int len)
{
  int i;
  unsigned char *p = b;
  i = 0;
  while(len > 0)
    {
      *p = c;
      p++;
      len--;
    }
  return(b);
}

static boolean bQueue_is_empty(const CallQueue_t* cpstrQueue) {
    return (cpstrQueue->u8Size == 0);
}

static boolean bQueue_is_full(const CallQueue_t* cpstrQueue) {
    return (cpstrQueue->u8Size == cu8CALL_QUEUE_SIZE);
}

static void vidQueue_enqueue(CallQueue_t* pstrQueue, uint8_t u8Floor, CallType_t enuCallType) {
    if(!bQueue_is_full(pstrQueue)) {
        pstrQueue->strCalls[pstrQueue->u8Tail].u8Floor = u8Floor;
        pstrQueue->strCalls[pstrQueue->u8Tail].enuCallType = enuCallType;
        pstrQueue->strCalls[pstrQueue->u8Tail].u16Timestamp = elevator_hal_u16Get_time_ms();
        pstrQueue->u8Tail = (pstrQueue->u8Tail + 1) % cu8CALL_QUEUE_SIZE;
        pstrQueue->u8Size++;
    }
}

static ElevatorCall_t* pstrQueue_peek(CallQueue_t* pstrQueue) {
    if(!bQueue_is_empty(pstrQueue)) {
        return &pstrQueue->strCalls[pstrQueue->u8Head];
    }
    return NULL;
}

static void vidQueue_dequeue(CallQueue_t* pstrQueue) {
    if(!bQueue_is_empty(pstrQueue)) {
        pstrQueue->u8Head = (pstrQueue->u8Head + 1) % cu8CALL_QUEUE_SIZE;
        pstrQueue->u8Size--;
    }
}

static void vidUpdate_led_state(uint8_t u8Floor, CallType_t enuCallType) {
    LedBlinkState_t* pstrLedState = &callHandler.strLedStates[u8Floor];
    pstrLedState->bIsBlinking = True;
    
    // Set duty cycle based on call type
    switch(enuCallType) {
        case CALL_INTERNAL:
        pstrLedState->u8DutyPercent = cu8LED_INT_DUTY_CYCLE;
            break;
            
        case CALL_EXTERNAL:
        pstrLedState->u8DutyPercent = cu8LED_EXT_DUTY_CYCLE;
            break;
            
        case CALL_BOTH:
            // Use internal call pattern as priority
            pstrLedState->u8DutyPercent = cu8LED_INT_DUTY_CYCLE;
            break;
            
        default:
        pstrLedState->bIsBlinking = False;
            break;
    }
}

static void vidProcess_led_blink(uint8_t u8Floor) {
    LedBlinkState_t* pstrLedState = &callHandler.strLedStates[u8Floor];
    uint16_t u16CurrentTime = elevator_hal_u16Get_time_ms();
    
    if(u16CurrentTime - pstrLedState->u16LastToggle >= cu16LED_BLINK_PERIOD_MS) {
        pstrLedState->bCurrentState = !pstrLedState->bCurrentState;
        pstrLedState->u16LastToggle = u16CurrentTime;
        
        // Apply duty cycle
        if(pstrLedState->bCurrentState) {
            uint16_t u16OnTime = (cu16LED_BLINK_PERIOD_MS * pstrLedState->u8DutyPercent) / 100;
            //elevator_hal_vidTimer_start(u16OnTime);
            //elevator_hal_vidSet_floor_led(u8Floor, True);
        }
        else {
            uint16_t u16OffTime = (cu16LED_BLINK_PERIOD_MS * (100 - pstrLedState->u8DutyPercent)) / 100;
            //elevator_hal_vidTimer_start(u16OffTime);
            //elevator_hal_vidSet_floor_led(u8Floor, False);
        }
    }
}