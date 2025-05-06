#ifndef MOTION_CONTROLLER_LCFG_H
#define MOTION_CONTROLLER_LCFG_H

#include "dio_types.h"
#if ! ((defined MotionController_c) || (defined MotionController_lcfg_c))
    #error "This file must not be included from outside of it's component"
#endif

#include "dio.h"

/* Relay Control Pins */
/* UP RELAY */
const GPIO_t cenuUpRelayPort = GPIOC;
const uint8_t cu8UP_RELAY_PIN = 4U;
const Dio_Cfg_t strUpRelay = {GPIOC, 4U, DIR_OUTPUT};

/* DN RELAY */
const GPIO_t cenuDnRelayPort = GPIOC;
const uint8_t cu8DN_RELAY_PIN = 3U;
const Dio_Cfg_t strDnRelay = {GPIOC, 3U, DIR_OUTPUT};

/* LS RELAY */
const GPIO_t cenuLsRelayPort = GPIOC;
const uint8_t cu8LS_RELAY_PIN = 5U;
const Dio_Cfg_t strHsRelay = {GPIOC, 6U, DIR_OUTPUT};

/* HS RELAY */
const GPIO_t cenuHsRelayPort = GPIOC;
const uint8_t cu8HS_RELAY_PIN = 6U;
const Dio_Cfg_t strLsRelay = {GPIOC, 5U, DIR_OUTPUT};

#endif // MOTION_CONTROLLER_LCFG_H
