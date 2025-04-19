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

// gpio status codes, representing differnt ways a gpio or i2c operation could complete.
// GPIO_SUCCESS represents a universal operation success,and the other statuses
// are meant to specify different error values an operation could have.
enum gpio_statuses {
    GPIO_SUCCESS,
    GPIO_TIMEOUT, // an operation timed out
    GPIO_ERR_MMAP, //memory map failed 
    GPIO_ERR_IO_I2C,// some IO related I2c error happened (for example, the )
    GPIO_ERR_MEMORY,
};

enum gpio_init_flags {
    GPIO_INIT_GPIO = 1,
    GPIO_INIT_I2C = 2,
    GPIO_INIT_ALL = 3,
}; 

/**
 * @brief Handles GPIO errors by printing an error message, cleaning up, and exiting.
 *
 * This function checks the result of a GPIO operation. If the operation failed 
 * (i.e., `resultant` is not equal to `GPIO_SUCCESS`), it logs an error message 
 * to `stderr` using the corresponding message from the `errs` array, calls 
 * `gpio_deinit` to clean up the GPIO state, and exits the program with `EXIT_FAILURE`.
 *
 * If the operation was successful, the function simply returns `GPIO_SUCCESS`.
 *
 * @param state Pointer to the current GPIO state structure.
 * @param resultant The result code from a previous GPIO operation.
 * @return Always returns `GPIO_SUCCESS` if the operation did not fail.
 *
 * @see gpio_statuses
 *
 * @example examples/example_exit_err.c
 */
int gpio_exit_err(gpio_state * state, int resultant);

/**
 * @brief initializes library functionality, populating the state structure.
 *
 * This function initializes the library's modules as specified by the user.
 * modules are generally represented by their own header file and initializing them
 * can be done by specifying any of the intialization flags.
 *
 * @param state Pointer to the gpio state structure (can be uninitialized)
 * @param init_flags bitwise flags specifying which modules to initialize
 * @return Returns GPIO success if all modules initialized 
 *
 * @see gpio_statuses
 */
int gpio_init(gpio_state * state, uflags init_flags);

/**
 * @brief denitiializes library resources, use before exiting program.
 *
 * This function deinitializes all of the libaries resources, and cleans up any initailized
 * modules, and as such is best called when the library is no longer needed. This should be smart
 * enough to detect whether the resources have actually been initialized, so you shouldn't have to worry
 * about double frees... probably...
 *
 * @param state initialized gpio state
 * @return  GPIO_SUCCESS if all modules were successfully uninitialized
 */
int gpio_deinit(gpio_state * state);

/**
 * @brief variadic function to initialize gpio pins for reading and writing...
 *
 * This function is in charge of initializing sets of GPIO pins to either GPIO_READ 
 * or GPIO_WRITE. This function is variadic, so it can take any number of pins, but it can 
 * only take a single option, so all the specified numbers 
 * will be initialised to the same read or write.
 *
 * @param state initialized gpio state 
 * @param mode the mode that the pins should be initialized to (either GPIO_INPUT or GPIO_OUTPUT)
 * @param n the number of gpio pins to initialize (this is because variadic functions in c)
 *  are JANKY AF
 * @return GPIO_SUCCESS if all of the pins were successfully initialized
 */
int gpio_init_pins(gpio_state * state, int mode, int n,...);

/**
 * @brief outputs a signal to a GPIO_OUTPUT initialized pin
 *
 * This function outputs a high signal to the specified pin number for the given
 * number of milliseconds. This will block the calling thread for the waiting time
 * so bewarb.
 *
 * @see gpio_init_pins
 * 
 * @param state initialized gpio state
 * @param pin_id the pin to output
 * @param ms how many milliseconds to output
 * @return GPIO_SUCCESS at all times unless it segfaults
 */
int gpio_output_pin_ms(
    gpio_state * state, 
    unsigned int pin_id, 
    unsigned int ms
);


/**
 * @brief spinlock function which awaits a pin change in state
 * 
 * This function blocks the current thread in a spinlock and
 * waits for the pin to go from HIGH to LOW. This blocks the current
 * thread in a polling while loop AKA a spinlock.
 *
 * This function's name is very appropriate and matches the 
 * functionality it implents and it is not funny.
 *
 * @param state initialized gpio state
 * @param pin_id pin to read input
 * @param timeout_ms number of milliseconds to wait before timing out.
 *  (do -1 for no timeout)
 * @param wait_time how much time was spent waiting in milliseconds
 * @return GPIO_SUCCESS if the pin changes from HIGH to LOW before timeout
 * or GPIO_TIMEOUT if timeout occurs.
 */
int gpio_await_ligmad(
    gpio_state * state, 
    int pin_id, 
    int timeout_ms,
    double * wait_time
);

/**
 * @brief sleep the calling thread for the specified number of milliseconds 
 *
 * This function sleeps the calling thread for the specified number of millseconds.
 * It works basically identically to any thread sleep function.
 * @param ms the number of milliseconds to sleep for
 * @return always GPIO_SUCCESS
 */
int gpio_sleep_ms(unsigned int ms);


/**
 * @brief read the current state of a GPIO_INPUT configured pin
 *
 * This function reads the current state (either LO or HIGH) 
 * of the configured input pin. Only works on GPIO_INPUT configured pins
 *
 *
 *
 * @param state initialized gpio state
 * @param pnr pin number
 * @return GPIO_SUCCESS at all times
 */
int gpio_get_input(gpio_state * state, unsigned int pnr);

#endif
