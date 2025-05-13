/* ************************************************************************ */
/* ************************************************************************ */
/*                        (C) Copyright 2025 by                             */
/*                              HexaMix                                     */
/*                         All rights reserved                              */
/*                                                                          */
/* ************************************************************************ */
/* ************************************************************************ */

/* ************************************************************************ */
/* ************************************************************************ */
/*                        FILE INCLUSION MANAGEMENT                         */
/* ************************************************************************ */
/* ************************************************************************ */
/* Protection against bad and multiple inclusions                           */
/* ************************************************************************ */

#ifndef RELAY_MANAGER_GCFG_H
#define RELAY_MANAGER_GCFG_H

/* ************************************************************************ */
/* ************************************************************************ */
/* Export of Relay pins definitions                                         */
/* ************************************************************************ */
/* ************************************************************************ */

#define cu8RELAY_COUNT ((uint8_t) 8U)

#define cenuRelayDOOR_Port      ((GPIO_t) GPIOC)
#define cu8RelayDOOR_Pin        ((uint8_t) 7U)
#define cenuRelayHS_Port        ((GPIO_t) GPIOC)
#define cu8RelayHS_Pin          ((uint8_t) 6U)
#define cenuRelayLS_Port        ((GPIO_t) GPIOC)
#define cu8RelayLS_Pin          ((uint8_t) 5U)
#define cenuRelayUP_Port        ((GPIO_t) GPIOC)
#define cu8RelayUP_Pin          ((uint8_t) 4U)
#define cenuRelayDN_Port        ((GPIO_t) GPIOC)
#define cu8RelayDN_Pin          ((uint8_t) 3U)
#define cenuRelayCAM_Port       ((GPIO_t) GPIOC)
#define cu8RelayCAM_Pin         ((uint8_t) 2U)
#define cenuRelayLIGHT_Port     ((GPIO_t) GPIOC)
#define cu8RelayLIGHT_Pin       ((uint8_t) 1U)
#define cenuRelayMNT_Port       ((GPIO_t) GPIOC)
#define cu8RelayMNT_Pin         ((uint8_t) 0U)

typedef enum {
    RELAY_DOOR = 0,
    RELAY_HS,
    RELAY_LS,
    RELAY_UP,
    RELAY_DN,
    RELAY_CAM,
    RELAY_LIGHT,
    RELAY_MNT,
    RELAY_ALL
} Relay_t;

/* ************************************************************************ */

#endif /* #ifndef RELAY_MANAGER_GCFG_H */

/* ************************************************************************ */
/* ************************************************************************ */
/*                           E N D   O F   F I L E                          */
/* ************************************************************************ */
/* ************************************************************************ */
