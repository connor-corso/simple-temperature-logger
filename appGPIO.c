// Standard Includes
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include <stdio.h>


// Custom Software Includes
#include "appDefinitions.h"
#include "appGPIO.h"

void gpio_irq_callback(uint gpio, uint32_t events)
{
    if (gpio == RESET_BUTTON) reset_function();
}

void reset_function(void)
{   
    // Wait 5ms and check again to see if the reset button is still held down (debouncing)
    busy_wait_ms(5);
    if (gpio_get(RESET_BUTTON) == 1)
    {
        watchdog_enable(1000, 0);
        printf("Reset Button Detected, going down!");
        turn_on_led(LED_WRN);
        turn_on_led(LED_FLT);
        turn_on_led(LED_ACTIVITY);
    }
    else
    {
        printf("Reset button flickering detected");
    }
}

void setup_interrupt(void)
{   
    // Setup the RESET_BUTTON as a software controlled input in a pull down configuration
    gpio_set_function(RESET_BUTTON, GPIO_FUNC_SIO);
    gpio_set_dir(RESET_BUTTON, 0);
    gpio_set_pulls(RESET_BUTTON, 0, 1); 

    printf("Reset button status: %d\n", gpio_get(RESET_BUTTON));

    // Setup the interrupt to trigger on a rising edge
    gpio_set_irq_enabled_with_callback(RESET_BUTTON, GPIO_IRQ_EDGE_RISE, true, &gpio_irq_callback);
}

// Sets up the LED pins and configures them as software controlled outputs
void setup_led_pins(void)
{
    // Set the function of all of the LEDs to software controlled
    gpio_set_function(LED_ACTIVITY, GPIO_FUNC_SIO);
    gpio_set_function(LED_WRN, GPIO_FUNC_SIO);
    gpio_set_function(LED_FLT, GPIO_FUNC_SIO);

    // Set the direction of all of the LEDs to output
    gpio_set_dir(LED_ACTIVITY, GPIO_OUT);
    gpio_set_dir(LED_WRN, GPIO_OUT);
    gpio_set_dir(LED_FLT, GPIO_OUT);

    // Set all LEDs to 0 (off)
    gpio_put(LED_ACTIVITY, 0);
    gpio_put(LED_WRN, 0);
    gpio_put(LED_FLT, 0);
}

// Turn on the passed LED
void turn_on_led(uint8_t led)
{
    gpio_put(led, 1);
}

// Turn off the passed LED
void turn_off_led(uint8_t led)
{
    gpio_put(led, 0);
}

// Toggle the passed LED
void toggle_led(uint8_t led)
{
    bool state = gpio_get(led);
    gpio_put(led, !state);
}