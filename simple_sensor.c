#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

//Networking includes
#include "pico/cyw43_arch.h"
#include "lwip/altcp.h"
#include "lwip/dns.h"
#include "setupWifi.h"
#include "request.h"

// Custom software includes
#include "lib/DHT20.h"


#ifdef DHT20_EXAMPLE_USE_SDA
#define I2C_SDA DHT20_EXAMPLE_USE_SDA
#else
#define I2C_SDA 12
#endif

#ifdef DHT20_EXAMPLE_USE_SCL
#define I2C_SDA DHT20_EXAMPLE_USE_SCL
#else
#define I2C_SCL 13
#endif

#ifdef USE_I2C_1
#define EXAMPLE_I2C_INST i2c1
#else
#define EXAMPLE_I2C_INST i2c0
#endif

// For logging data to the server
#define BUF_SIZE 2048
#define SLEEP_TIME_BETWEEN_READINGS_IN_MS 60 * 1000 // Amount of time to sleep for between taking readings
#define IP_REFRESH_TIMER 15 //Number of times that we will sleep for SLEEP_TIME_BETWEEN_READINGS_IN_MS before refreshing the IP address

void transmit_data(int, float, ip_addr_t);
ip_addr_t set_ip(void);

DHT20 sens;
DHT20 *sens_ptr = &sens;

uint32_t getTotalHeap(void) 
{
   extern char __StackLimit, __bss_end__;
   
   return &__StackLimit  - &__bss_end__;
}

uint32_t getFreeHeap(void) 
{
   struct mallinfo m = mallinfo();

   return getTotalHeap() - m.uordblks;
}

void print_free_memory(void)
{
    uint32_t total, free, used;
    total = getTotalHeap();
    free = getFreeHeap();
    used = total - free;
    printf("\nTotal heap: %d\nFree heap: %d\nUsed: %d\n", getTotalHeap, getFreeHeap, used);
}


int setup_dht20()
{
    // Set up stdio
    stdio_init_all();
    printf("\033[2J");
#ifndef DHT20_EXAMPLE_SKIP_INIT_SLEEP
    sleep_ms(2000);
#endif
    printf("Running controller setup.\n");
    printf("Setting up i2c\n");
    // Setup i2c @ 100k
    i2c_init(EXAMPLE_I2C_INST, 100000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_SDA); //hardware pull up resistors are used
    //gpio_pull_up(I2C_SCL);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA, I2C_SCL, GPIO_FUNC_I2C));
    printf("Set up i2c.\n");

    // Set up temp and humid sensor
    printf("Initialize DHT20.\n");
    int sensor_ret = DHT20_init(sens_ptr);
    if (sensor_ret != DHT20_OK)
    {
        printf("Failed to initialize the sensor.\n");
        printf("Sensor return value %d\n", sensor_ret);
        return sensor_ret;
    }
    printf("Initialized DHT20.\n");

    // Sleep for 10ms after successful sensor initialization
    sleep_ms(10);
    return 0;
}

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
        print_free_memory();
        sleep_ms(SLEEP_TIME_BETWEEN_READINGS_IN_MS);
    }

    return 0;
}

void dns_found(const char *name, const ip_addr_t *ip, void *arg)
{
    ip_addr_t *ipResult = (ip_addr_t *)arg;
    if (ip)
    {
        ip4_addr_copy(*ipResult, *ip);
    }
    else
    {
        ip4_addr_set_loopback(ipResult);
    }
    return;
}

err_t getIP(char *URL, ip_addr_t *ipResult)
{
    cyw43_arch_lwip_begin();
    err_t err = dns_gethostbyname(URL, ipResult, dns_found, ipResult);
    cyw43_arch_lwip_end();
    return err;
}


int log_data_to_server(ip_addr_t ip, float data, char *output_buffer, int station)
{
    char buf[50];
    char hostname[] = "monitoring-backend.ccorso.ca";
    memset(buf, 0, sizeof(buf));
    memset(output_buffer, 0, 2048);
    snprintf(buf, sizeof(buf), "/record/%u/?data=%.2f", station, data);
    printf("/record/%u/?data=%.2f", station, data);
    //struct connectionState *cs1 = doRequest(&ip, hostname, 2710, "POST", buf, NULL, myBuff1);
    struct connectionState *cs1 = doRequest(&ip, "monitoring-backend.ccorso.ca", 2710, "POST", buf, NULL, output_buffer);
    
    while (pollRequest(&cs1))
    {
        sleep_ms(100);
        //printf("."); // print a "." to indicate that the system is not stuck
    }
    return 0;
}

ip_addr_t set_ip()
{
    ip_addr_t ip;
    ip4_addr_set_zero(&ip);
    getIP("monitoring-backend.ccorso.ca", &ip);
    //getIP("jellyfin.ccorso.ca", &ip); // Use jellyfin as that will get the home IP address instead of the cloudflare one
    while (!ip_addr_get_ip4_u32(&ip))
    {
        sleep_ms(100);
    };
    if (ip4_addr_isloopback(&ip))
    {
        printf("address not found");
        return ip;
    }
    return ip;
}


void transmit_data(int station, float data, ip_addr_t ip)
{
    // Setup a buffer
    char myBuff1[BUF_SIZE];
    
    // Log the data to the server
    log_data_to_server(ip, data, myBuff1, station);

    // Print the results
    printf("Data: %f, station: %d. \nUpload results: %s", data, station, myBuff1);
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