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
    // Make sure that the error id being reported is valid
    if (error_id >= MAX_NUM_ERRORS)
    {
        // If the error isn't valid, then report an invalid error code error
        report_error(INVALID_ERROR_CODE);
        return;
    }
    #ifdef DEBUG_MODE
        printf("\nError ID: %d\nError severity: %d\n",error_id, error_codes[error_id].error_severity);
    #endif // DEBUG_MODE

    // Determine the appropriate action to take
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
    
    // Set the error_status to active
    error_status[error_id] = ERROR_STATUS_ACTIVE;
}


void clear_error(uint32_t error_id)
{

}