#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "hardware/watchdog.h"

// Custom software includes
#include "appDefinitions.h"
#include "lib/DHT20.h"
#include "appGPIO.h"
#include "memoryInfo.h"
#include "networkCommunication.h"
#include "sensors.h"
#include "simple_sensor.h"
#include "errors.h"


int loop()
{
    int ret = 0;
    uint32_t count = 1, ip_expiry_count = 0;
    printf("Starting the temperature, humidity fetch loop.\n\n");

    // get the ip address from DNS
    ip_addr_t ip = set_ip();
    print_ip_address(ip);

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
            printf("--- Humidity: %5.2f %%RH\n\n", getHumidity(sens_ptr));
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
        
        // Sleep for SLEEP_TIME_BETWEEN_READINGS_IN_MS in four parts
        sleep_and_toggle_led(SLEEP_TIME_BETWEEN_READINGS_IN_MS, 6);
        //sleep_ms(SLEEP_TIME_BETWEEN_READINGS_IN_MS);
    }
    return 0;
}

// Sleep for the total duration in parts, toggling the LED after each part sleep
void sleep_and_toggle_led(uint32_t sleep_time, uint8_t parts)
{
    for (uint8_t i = 0; i < parts; i++)
    {
        //sleep_ms(sleep_time / parts);
        sleep_ms(sleep_time / parts);
        toggle_led(LED_ACTIVITY);
    }
}


int main()
{
    int ret;

    // Set up stdio
    stdio_init_all();

    // Clear the screen
    printf("\033[2J");

    // Setup the LED GPIO pins
    setup_led_pins();

    // Check to see if the watchdog caused the reboot
    if (watchdog_caused_reboot())
    {
        // Print a notice and report the error
        printf("\nRebooted by Watchdog\n");
        report_error(WATCHDOG_RESET_DETECTED);
    }

    // Setup all of the sensors
    ret = setup_all_sensors();

    // Setup the interrupt on the button
    setup_interrupt();

    // Connect to the wifi network
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