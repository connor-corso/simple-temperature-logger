#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// Custom software includes
#include "appDefinitions.h"
#include "lib/DHT20.h"
#include "appGPIO.h"
#include "memoryInfo.h"
#include "networkCommunication.h"
#include "sensors.h"


int loop()
{
    int ret = 0;
    uint32_t count = 1, ip_expiry_count = 0;
    printf("Starting the temperature, humidity fetch loop.\n");

    // get the ip address from DNS
    ip_addr_t ip = set_ip();
    printf("Got IP address");

    while (true)
    {
        // If the IP has been used for more than IP_REFRESH_TIMER, then get it again
        if (ip_expiry_count > IP_REFRESH_TIMER)
        {
            // Refresh the IP address
            ip = set_ip();
            // Reset the count
            ip_expiry_count = 0;
        }

        // Take a measurement
        ret = getMeasurement(sens_ptr);
        if (ret != DHT20_OK)
        {
            // Measurement failed
            printf("Measurement %d failed with error value %d\n", count, ret);
            printf("Trying again after 10s...\n");
        }
        else
        {
            // Measurement was successful, print info and transmit that information
            printf("\nMeasurement number: %d\n", count);
            printf("--- Temperature: %5.2f C°\n", getTemperature(sens_ptr));
            printf("--- Humidity: %5.2f \%RH\n\n", getHumidity(sens_ptr));
            transmit_data(4, getTemperature(sens_ptr), ip);
            transmit_data(5, getHumidity(sens_ptr), ip);
        }

        // Increment the IP expiry count
        ip_expiry_count++;
        // Increment the count of total measurements
        count++;

        // Print the amount of free memory
        #ifdef DEBUG_PRINT_MEMORY_INFO
            print_free_memory();
        #endif // DEBUG_MODE
        
        // Sleep for SLEEP_TIME_BETWEEN_READINGS_IN_MS
        sleep_ms(SLEEP_TIME_BETWEEN_READINGS_IN_MS);
    }

    return 0;
}


int main()
{
    int ret;
    // Setup the dht20
    ret = setup_dht20();

    // Connect to the network
    connect();

    // In case we add failure conditions after setup, handle them after
    if (!ret)
    {
        return loop();
    }
    else{
        printf("Failed to start loop.\n");
        return ret;
    }
}