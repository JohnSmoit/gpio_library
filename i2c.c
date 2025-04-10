/***************************************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Ryan Algazi
* Student ID:: 921914536
* Github-Name:: JohnSmoit
* Project:: Assignment 2 - Tapeless Ruler
*
* File:: i2c.h
*
* Description::
*   Defines i2c helper functions for interfacing with the pi's
*   driver
**************************************************************/
#include "i2c.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/i2c-dev.h>

#include "gpio.h"

typedef struct gpio_i2c_state {
    int fd;
} gpio_i2c_state;

int i2c_init(gpio_state * state) {
    // TODO: replace with custom allocator at some point
    state->i2c = (gpio_i2c_state *) malloc(sizeof(gpio_i2c_state));
    if (!state->i2c) {
        return GPIO_ERR_MEMORY;
    }

    // TODO: Open/close per read/write so other processes can use this file
    if ((state->i2c->fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        return GPIO_ERR_IO_I2C;
    }



    return GPIO_SUCCESS;
}


int i2c_deinit(gpio_state * state) {
    if (state->i2c == NULL) {
        return GPIO_SUCCESS;
    }
    // obviously we init this to null so this doesn't shit
    // itself
    if (close(state->i2c->fd) != 0) {
        return GPIO_ERR_IO_I2C;
    }

    free(state->i2c);
    state->i2c = NULL;

    return GPIO_SUCCESS;
}

int i2c_write_bytes(gpio_state * state, const u8 * buf, usize len) {
    u8 reg = *buf;

    if (write(state->i2c->fd, buf, len) == -1) {
        return GPIO_ERR_IO_I2C;
    }

    return GPIO_SUCCESS;
}

int i2c_read_bytes(gpio_state * state, u8 reg, u8 * buf, usize len) {

    u8 wbuf[1] = {reg};
    write(state->i2c->fd, wbuf, 1);
    if (read(state->i2c->fd, buf, len) != -1) {
        return GPIO_ERR_IO_I2C;
    }

    return GPIO_SUCCESS;
}

int i2c_init_slave_address(gpio_state * state, u8 address) {

    if (ioctl(state->i2c->fd, I2C_SLAVE, address) < 0) {
        return GPIO_ERR_IO_I2C;
    }
    
    return GPIO_SUCCESS;
}
