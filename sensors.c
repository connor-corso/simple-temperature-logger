#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lib/DHT20.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "sensors.h"
#include "appDefinitions.h"

DHT20 sens;
DHT20 *sens_ptr = &sens;

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
