/***************************************************************
* Class:: CSC-615-01 Spring 2025
* Name:: Ryan Algazi
* Student ID:: 921914536
* Github-Name:: JohnSmoit
* Project:: Final Project -- THE CAR
*
* File:: i2c.h
*
* Description::
*   Defines i2c helper functions for interfacing with the pi's
*   driver
**************************************************************/
#include "core/i2c.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/i2c-dev.h>

#include "core/gpio.h"
#include "types.h"
#include "log.h"

#define MAX_I2C_HANDLES 20
#define MAX_I2C_FILES 20

struct _i2c_handle_slot {
    gpio_i2c_state parent;
    i32 i2c_fd;

    u8 slave_address;
    u8 is_free;
};

struct _gpio_i2c_state {
    struct _i2c_handle_slot i2c_handles[MAX_I2C_HANDLES];

    i32 next_handle;
};


u8 i2c_get_address(i2c_handle_slot slot) {
    return slot->slave_address;
}

u8 i2c_init(gpio_i2c_state * st) {
    // TODO: replace with custom allocator at some point
    gpio_i2c_state state = *st;
    
    state = (gpio_i2c_state)malloc(sizeof(struct _gpio_i2c_state));
    state->next_handle = 0;


    for (i32 i = 0; i < MAX_I2C_HANDLES; i++) {
        state->i2c_handles[i].is_free = TRUE;
        state->i2c_handles[i].parent = state;
        state->i2c_handles[i].i2c_fd = -1;
    }

    car_log_info("Did a thing (i2c init)!\n");
    *st = state;
    return GPIO_SUCCESS;
}


u8 i2c_deinit(gpio_i2c_state * st) {
    gpio_i2c_state state = *st;

    for (i32 i = 0; i < MAX_I2C_HANDLES; i++) {
        i2c_handle_slot slot = &state->i2c_handles[i];

        i2c_free_device_handle(slot);
    }

    free(state);
    *st = INVALID_HANDLE_VALUE;

    return GPIO_SUCCESS;
}

u8 i2c_begin(i2c_handle_slot slot) {
    // if (slot->i2c_fd > 0 || slot->is_free) {
    //     car_log_error("I2C device file already opened! Pretty pleaze call i2c_end first,\n");
    //     car_log_error("I2C fd: %d\nfree: %d\n", slot->i2c_fd, slot->is_free);
    //     return GPIO_ERR_I2C_ORDER;
    // }

    car_log_info("Set controller address: %x fd: %d\n", slot->slave_address, slot->i2c_fd);

    return GPIO_SUCCESS;
}

u8 i2c_end(i2c_handle_slot slot) {
    // if (slot->i2c_fd <= 0) {
    //     car_log_error("I2C device file never opened! Please call i2c_begin first.");
    //     return GPIO_ERR_I2C_ORDER;
    // }

    return GPIO_SUCCESS;
}

u8 i2c_write_bytes(i2c_handle_slot slot, u8 reg, const u8 * buf, usize len) {
    //u8 reg = *buf;
    
    u8 rbuf[len + 1];
    rbuf[0] = reg;
    memcpy(rbuf + 1, buf, len);

    //i2c_begin(slot);
    if (write(slot->i2c_fd, rbuf, len + 1) == -1) {
        car_log_error("Failed to write stuff to i2c device %x (fd = %d)\n", slot->slave_address, slot->i2c_fd);

        return GPIO_ERR_IO_I2C;
    }
    //i2c_end(slot);

    return GPIO_SUCCESS;
}


u8 i2c_write_single(i2c_handle_slot slot, u8 reg, u8 buf) {
    i2c_write_bytes(slot, reg, &buf, 1);

    return GPIO_SUCCESS;
}

u8 i2c_read_bytes(i2c_handle_slot slot, u8 reg, u8 * buf, usize len) {

    u8 wbuf[1] = {reg};
    
    i2c_begin(slot);
    write(slot->i2c_fd, wbuf, 1);
    if (read(slot->i2c_fd, buf, len) != -1) {
        car_log_error("Failed to read bytes, either the write or read in this function failed.");
        return GPIO_ERR_IO_I2C;
    }

    i2c_end(slot);

    return GPIO_SUCCESS;
}

i2c_handle_slot i2c_get_device_handle(u8 address, gpio_i2c_state state) {
    i32 ind = 0;
    
    car_log_info("Starting handle acquisition turd turd\n");
    for (; ind < MAX_I2C_HANDLES && !state->i2c_handles[ind].is_free; ind++) {
        car_log_info("Handle %d is not free!\n", ind);
    }
        

    if (ind == MAX_I2C_HANDLES) {
        car_log_error("No handles remaining for addr: %d, maybe give some back or smth.", address);
        return INVALID_HANDLE_VALUE;
    }

    i2c_handle_slot slot = &state->i2c_handles[ind];

    if ((slot->i2c_fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        car_log_error("I2C device file failed to open. WOMP WOMP.\n");
        return INVALID_HANDLE_VALUE;
    }
    
    car_log_info("Problems: %d\n", slot->i2c_fd);
    if (ioctl(slot->i2c_fd, I2C_SLAVE, address) < 0) {
        car_log_error("Failed to set slave/controller address %d\n", slot->slave_address);
        return INVALID_HANDLE_VALUE;
    }

    slot->is_free = FALSE;
    slot->slave_address = address;

    return slot;
}

void i2c_free_device_handle(i2c_handle_slot slot) {
    slot->is_free = TRUE;

    if (slot->i2c_fd > 0 && close(slot->i2c_fd) != 0) {
        car_log_error("I2c Device failed to close, this is probably bad.\n");
    }

    slot->i2c_fd = -1;
}
