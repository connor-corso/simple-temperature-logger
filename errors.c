// Standard Includes
#include <stdio.h>

// Custom Software Includes
#include "errors.h"
#include "appGPIO.h"
#include "appDefinitions.h"


// Array definitions
error_code_t error_codes[MAX_NUM_ERRORS] = 
{
    {0,SEVERITY_LEVEL_LOG_ONLY},
    {DHT_20_SENSOR_ERROR, SEVERITY_LEVEL_CRITICAL},
    {NETWORK_CONNECTION_ERROR, SEVERITY_LEVEL_CRITICAL},
    {NETWORK_REQUEST_ERROR, SEVERITY_LEVEL_CRITICAL},
    {INVALID_ERROR_CODE, SEVERITY_LEVEL_WARNING},
    {INVALID_ERROR_STATUS, SEVERITY_LEVEL_WARNING},
    {WATCHDOG_RESET_DETECTED, SEVERITY_LEVEL_WARNING}
};

uint8_t error_status[MAX_NUM_ERRORS] = {0};


// Report an error, for now this will just light up the related LED
void report_error(uint32_t error_id)
{
    printf("Error severity: %d\n", error_codes[error_id].error_severity);
    switch (error_codes[error_id].error_severity)
    {
        case SEVERITY_LEVEL_CRITICAL:
            turn_on_led(LED_FLT);
            break;
        
        case SEVERITY_LEVEL_WARNING:
            printf("Warning error detected");
            turn_on_led(LED_WRN);
            break;

        case SEVERITY_LEVEL_LOG_ONLY:
           break;

        default:
            report_error(INVALID_ERROR_STATUS);
            break;
    }
}


void clear_error(uint32_t error_id)
{

}