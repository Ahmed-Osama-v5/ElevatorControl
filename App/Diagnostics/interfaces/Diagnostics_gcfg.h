#ifndef DIAGNOSTICS_GCFG_H
#define DIAGNOSTICS_GCFG_H

/* Event Log Configuration */
#define cu8MAX_EVENT_LOG_SIZE        ((uint8_t) 50U)
#define cu8MAX_ERROR_COUNT           ((uint8_t) 10U)

/* Maintenance Thresholds */
#define cu16DOOR_CYCLES_MAINTENANCE  ((uint16_t) 5000U)
#define cu16TRAVEL_CYCLES_MAINTENANCE ((uint16_t) 10000U)

/* Health Check Intervals */
#define cu16HEALTH_CHECK_INTERVAL_MS ((uint16_t) 1000U)

/* Error Debounce Time */
#define cu16ERROR_DEBOUNCE_MS        ((uint16_t) 100U)

/* Component Status Bits */
#define cu8COMP_STATUS_NORMAL        ((uint8_t) 0x00U)
#define cu8COMP_STATUS_WARNING       ((uint8_t) 0x01U)
#define cu8COMP_STATUS_ERROR         ((uint8_t) 0x02U)
#define cu8COMP_STATUS_CRITICAL      ((uint8_t) 0x03U)

/* Diagnostic Error Codes */
typedef enum {
    DIAG_ERR_NONE = 0,
    DIAG_ERR_DOOR_TIMEOUT,
    DIAG_ERR_MOTION_TIMEOUT,
    DIAG_ERR_OVERLOAD,
    DIAG_ERR_PHASE_FAULT,
    DIAG_ERR_LOCK_FAULT,
    DIAG_ERR_STOP_ACTIVATED,
    DIAG_ERR_LIMIT_SWITCH,
    DIAG_ERR_MAINTENANCE_NEEDED,
    DIAG_ERR_SENSOR_FAULT,
    DIAG_ERR_RELAY_FAULT,
    DIAG_ERR_COMM_FAULT
} DiagError_t;

/* Diagnostic Event Types */
typedef enum {
    DIAG_EVENT_ERROR_OCCURRED,
    DIAG_EVENT_ERROR_CLEARED,
    DIAG_EVENT_MAINTENANCE_DUE,
    DIAG_EVENT_SYSTEM_RESET,
    DIAG_EVENT_MODE_CHANGE,
    DIAG_EVENT_DOOR_CYCLE,
    DIAG_EVENT_FLOOR_REACHED
} DiagEvent_t;

/* System Component IDs */
typedef enum {
    DIAG_COMP_MOTION_CTRL,
    DIAG_COMP_DOOR_CTRL,
    DIAG_COMP_CALL_HANDLER,
    DIAG_COMP_MODE_MANAGER,
    DIAG_COMP_LED_CTRL,
    DIAG_COMP_SENSOR_MANAGER
} DiagComponent_t;


#endif // DIAGNOSTICS_GCFG_H
