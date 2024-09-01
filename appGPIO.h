#include "pico/stdlib.h"

// Function declarations
void gpio_irq_callback(uint, uint32_t);
void setup_interrupt(void);
void reset_function(void);

void setup_led_pins(void);
void turn_on_led(uint8_t);
void turn_off_led(uint8_t);
void toggle_led(uint8_t);