// Standard Includes
#include "pico/stdlib.h"

// Custom Software Includes
#include "appDefinitions.h"
#include "appGPIO.h"

void gpio_irq_callback(uint gpio, uint32_t events)
{
    if (gpio == RESET_BUTTON) reset_function();
}

void reset_function()
{
    
}

void setup_interrupt()
{
    gpio_set_irq_enabled_with_callback(RESET_BUTTON, GPIO_IRQ_EDGE_RISE, true, &gpio_irq_callback);
}