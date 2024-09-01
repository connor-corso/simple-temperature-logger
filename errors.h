// Standard Includes
#include "pico/stdlib.h"

// Error related definitions
#define MAX_NUM_ERRORS 10

#define ERROR_STATUS_ACTIVE 1
#define ERROR_STATUS_INACTIVE 0

#define DHT_20_SENSOR_ERROR 1
#define NETWORK_CONNECTION_ERROR 2
#define NETWORK_REQUEST_ERROR 3
#define INVALID_ERROR_CODE 4
#define INVALID_ERROR_STATUS 5
#define WATCHDOG_RESET_DETECTED 6

// Function declarations
void report_error(uint32_t);
void clear_error(uint32_t);

// Type definitions
typedef struct error_code_t
{
    uint32_t error_id;
    uint8_t error_severity;
} error_code_t;

typedef enum
{
    SEVERITY_LEVEL_LOG_ONLY = 0,
    SEVERITY_LEVEL_WARNING = 1,
    SEVERITY_LEVEL_CRITICAL = 2
} ERROR_SEVERITY;