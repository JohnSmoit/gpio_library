/**************************************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Ryan Algazi
* Student ID:: 921914536
* Github-Name:: JohnSmoit
* Project:: Assignment 2 - Tapeless Ruler
*
* File:: gpio.c
*
* Description::
*   This is a basic library which
*   Implements various basic GPIO functions such as
*   input, output, and triggering signals...
**************************************************************/
#include "gpio.h"
#include "i2c.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

/*
 * Define for GPIO pin addresses in physical memory
 * (wow embedded programming yay!)
 *
 * The base address changes depending on architecture
 * */
#define PERIPHERAL_BASE_ADDRESS 0x3F000000
#define GPIO_BASE (PERIPHERAL_BASE_ADDRESS + 0x200000)

#define BLOCK_SIZE (4 * 1024)
#define PAGE_SIZE BLOCK_SIZE

/**
 * Helper macros for initializing, and actually setting GPIO state
 * */
#define INP_GPIO(st, g) *(st->gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(st, g) *(st->gpio+((g)/10)) |=  (1<<(((g)%10)*3))

#define GPIO_SET(st) *((st)->gpio+7)
#define GPIO_GET(st, g) ((*(st->gpio+13)&(1 << g)) >> g)
#define GPIO_CLR(st) *(st->gpio+10)

const char * errs[] = {
    [GPIO_SUCCESS] = "Everything totally worked",
    [GPIO_ERR_MMAP] = "Failed to map GPIO Addresses",
    [GPIO_TIMEOUT] = "Pin state changed timed out",
    [GPIO_ERR_IO_I2C] = "Failed to perform I2C-related IO",
    [GPIO_ERR_MEMORY] = "Failed to allocate memory",
};


/**
 * Helper functions.
 * Prepares the specified pin number to write output/read input.
 * */
void setup_pin_out(gpio_state * state, unsigned int pnr) {
    INP_GPIO(state, pnr);
    OUT_GPIO(state, pnr);
}

void setup_pin_in(gpio_state * state, unsigned int pnr) {
    INP_GPIO(state, pnr);
}

int gpio_exit_err(gpio_state * state, int resultant) {
    if (resultant != GPIO_SUCCESS) {
        const char * msg = errs[resultant];

        fprintf(
            stderr, 
            "GPIO Operation failed with code %d.\nReason: %s\n", 
            resultant,
            msg
        );

        gpio_deinit(state);
        exit(EXIT_FAILURE);
    }
    
    return GPIO_SUCCESS;
}

void bprint(int state);

int gpio_output_pin_ms(
    gpio_state * state,
    unsigned int pnr, 
    unsigned int time_ms
) {
    GPIO_SET(state) = 1 << pnr;
    gpio_sleep_ms(time_ms);
    GPIO_CLR(state) = 1 << pnr;

    return GPIO_SUCCESS;
}

void bprint(int state) {
    for (int i = 0; i < 32; i++) {
        putc('0' + (((1 << i) & state) >> i), stdout);

        putc(' ', stdout);
        putc(' ', stdout);
    }

    putc('\n', stdout);

    for (int i = 0; i < 32; i++) {
        printf("%02d ", i);
    }

    putc('\n', stdout);
}

int gpio_init_pins(gpio_state * state, int mode, int n, ...) {
    va_list args;
    va_start(args, n);
    
    for (int i = 0; i < n; i++) {
        int arg = va_arg(args, int);
        
        if (mode == GPIO_INPUT) {
            setup_pin_in(state, arg);
        } else {
            setup_pin_out(state, arg);
        }
    }

    va_end(args);

    return GPIO_SUCCESS;
}

int gpio_get_input(gpio_state * state, unsigned int pnr) {
    return GPIO_GET(state, pnr);
}


int gpio_sleep_ms(unsigned int milliseconds) {
    struct timespec ts = {
        .tv_sec = milliseconds / 1000,
        .tv_nsec = (milliseconds % 1000) * 1000000
    };

    return nanosleep(&ts, NULL);
}

time_t time_ms(void) {
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);

    return (tv.tv_sec * 1000) + tv.tv_nsec / 1000000;
}

time_t time_us(void) {
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    return (tv.tv_sec * 1000) + tv.tv_nsec / 1000;
}

int gpio_await_ligmad(
    gpio_state * state, 
    int pin_id, 
    int timeout_ms,
    double * wait_time
) {
    // spins until either the timeout interval has been reached
    // or the pin changes state, effectively tracking the time
    // that the pin stays in a certain state in milleseconds.

    while (gpio_get_input(state, pin_id) == 0);

    time_t tv = time_us();

    if (timeout_ms == -1) {
        goto FUCK;
    }
    while (time_us() - tv < timeout_ms * 1000) {
        if (gpio_get_input(state, pin_id) != 1) {
            if (wait_time)
                *wait_time = (double)(time_us() - tv) / 1000.0;
            return GPIO_SUCCESS;
        }
    }

    return GPIO_TIMEOUT;
FUCK:
    while(1) {
        if (gpio_get_input(state, pin_id) != 1) {
            if (wait_time)
                *wait_time = (double)(time_us() - tv) / 1000.0;
            return GPIO_SUCCESS;
        }
    }

    // this code is literally unreachable
    return GPIO_SUCCESS;
}

// partial initialization routines
int gpio_init_memory(gpio_state *);
int gpio_init_i2c(gpio_state *);

typedef int (*gpio_lifecycle_routine)(gpio_state *);

int gpio_init(gpio_state * state, uflags init_flags) {

    state->i2c = NULL;

    static gpio_lifecycle_routine init_table[GPIO_INIT_COUNT] = {
        [0] = gpio_init_memory,
        [1] = gpio_init_i2c,
    };

    state->init_flags = init_flags;

    for (int i = 0; i < GPIO_INIT_COUNT; i++) {
        int flag_bit = init_flags & 1;
        if (!flag_bit) continue;

        int res = init_table[i](state);
        if (res != GPIO_SUCCESS) {
            return res;
        }

        printf("Successfully ran routine %d\n", i);
    }
    return GPIO_SUCCESS;
}

int gpio_init_memory(gpio_state * state) {
    int mem_fd;
    void * gpio_map;

    state->gpio = 0;
    /**
     * Opens /dev/mem so that we can bring GPIO register addresses
     * into the program's address space.
     *
     * /dev/mem is a special device file that gives access to the physical memory of 
     * the pi. Since peripherals (including GPIO pins) have a specific physical
     * address, this file is used to access them. (if we can open it, that is)
     * */
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        return GPIO_ERR_MMAP;
    }

    /**
     * Here we use mmap to actually bring those physical addresses
     * into the program's address space so that we can read/write to them.
     * */
    gpio_map = mmap(
        NULL,
        BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mem_fd,
        GPIO_BASE
    );

    // we don't need the file anymore after we've successfully mapped the memory.
    close(mem_fd);
    if (gpio_map == MAP_FAILED) {
        return GPIO_ERR_MMAP;
    }

    /**
     * Finalize the mapping to a static variable.
     * It's volatile since it could change due to external factors.
     * */
    state->gpio = (volatile unsigned *) gpio_map;
    return GPIO_SUCCESS;
}

int gpio_init_i2c(gpio_state * state) {
    return i2c_init(state);
}

int gpio_memory_deinit(gpio_state * state);
int gpio_i2c_deinit(gpio_state * state);

int gpio_deinit(gpio_state * state) {
    static gpio_lifecycle_routine deinit_table[GPIO_INIT_COUNT] = {
        [0] = gpio_memory_deinit,
        [1] = gpio_i2c_deinit,
    };


    uflags init_flags = state->init_flags;
    for (int i = 0; i < GPIO_INIT_COUNT; i++) {
        int state_bit = init_flags & 1;
        if (state_bit) { 
            // program fails to deinitialize the feature
            // so we exit ungracefully
            int res = deinit_table[i](state);
            if (res != GPIO_SUCCESS) {
                return res;
            }
        }
    }

    return GPIO_SUCCESS;
}

int gpio_memory_deinit(gpio_state * state) {
    if (state->gpio) {
        munmap((unsigned *)state->gpio, BLOCK_SIZE);
    }

    return GPIO_SUCCESS;
}

int gpio_i2c_deinit(gpio_state * state) {
    return i2c_deinit(state);
}
