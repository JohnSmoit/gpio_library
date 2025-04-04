/**************************************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Ryan Algazi
* Student ID:: 921914536
* Github-Name:: JohnSmoit
* Project:: Assignment 2 - Tapeless Ruler
*
* File:: gpio.h
*
* Description::
*   This is a basic library which
*   Implements various basic GPIO functions such as
*   input, output, and triggering signals...
**************************************************************/

#ifndef GPIO_H
#define GPIO_H

#include "types.h"

#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

typedef struct gpio_i2c_state gpio_i2c_state;

typedef struct gpio_state {
    volatile unsigned * gpio;
    uflags init_flags;

    gpio_i2c_state * i2c;
} gpio_state;




/// exits in failure when the resultant value is not 
/// GPIO_SUCCESS.
int gpio_exit_err(gpio_state * state, int resultant);

/// Initializes GPIO memory, mainly being the virtual memory map
/// as of right now.
int gpio_init(gpio_state * state, uflags init_flags);

/// Deinitializes GPIO memory, if it has been initialized.
int gpio_deinit(gpio_state * state);

/// Initializes all of the pin numbers passed to the specified mode.
int gpio_init_pins(gpio_state * state, int mode, int n,...);

/// Sets the specified pin to high for the specified interval in milliseconds
int gpio_output_pin_ms(
    gpio_state * state, 
    unsigned int pin_id, 
    unsigned int ms
);

/// Waits for the given configured input pin to change from high state to low
/// Additionally, waits until the pin goes from low to high before measuring time.
int gpio_await_ligmad(
    gpio_state * state, 
    int pin_id, 
    int timeout_ms,
    double * wait_time
);

/// Sleeps the calling thread for the specified number of miliseconds
int gpio_sleep_ms(unsigned int ms);

/// Gets the current state of the specfied pin so long as it is configured
/// as an input pin.
int gpio_get_input(gpio_state * state, unsigned int pnr);

#endif
