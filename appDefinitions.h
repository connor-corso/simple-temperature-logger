// Pin definitions
#define RESET_BUTTON 4
#define LED_ACTIVITY 8
#define LED_WRN 9
#define LED_FLT 10
#define LIGHT_INTENSITY 28
#define LIGHT_INTENSITY_ANALOG_CHANNEL 2 // GP28_A2

// AHT20 sensor config
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

// Debug settings
//#define DEBUG_MODE
//#define DEBUG_PRINT_MEMORY_INFO
