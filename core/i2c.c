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
    u8 slave_address;
    u8 is_free;
    
    gpio_i2c_state parent;
};

struct _gpio_i2c_state {
    struct _i2c_handle_slot i2c_handles[MAX_I2C_HANDLES];

    i32 i2c_fd;   
    i32 next_handle;
};


u8 i2c_init(gpio_i2c_state state) {
    // TODO: replace with custom allocator at some point
    
    state->i2c_fd = -1;
    state->next_handle = 0;
    for (i32 i = 0; i < MAX_I2C_HANDLES; i++) {
        state->i2c_handles[i].is_free = FALSE;
        state->i2c_handles[i].parent = state;
    }
    return GPIO_SUCCESS;
}


u8 i2c_deinit(gpio_i2c_state state) {
    if (state->i2c_fd >= 0 || close(state->i2c_fd) != 0) {
        return GPIO_ERR_IO_I2C;
    }

    return GPIO_SUCCESS;
}

u8 i2c_begin(i2c_handle_slot slot) {
    if (slot->parent->i2c_fd > 0 || slot->is_free) {
        car_log_error("I2C device file already opened! Pretty pleaze call i2c_end first,\n");
        return GPIO_ERR_I2C_ORDER;
    }

    if ((slot->parent->i2c_fd = open("/dev/i2c-1", O_RDWR)) < 0) {
        car_log_error("I2C device file failed to open. WOMP WOMP.\n");
        return GPIO_ERR_IO_I2C;
    }

    if (ioctl(slot->parent->i2c_fd, I2C_SLAVE, slot->slave_address) == -1) {
        car_log_error("Failed to set slave/controller address %d\n", slot->slave_address);
        return GPIO_ERR_IO_I2C;
    }
    return GPIO_SUCCESS;
}

u8 i2c_end(i2c_handle_slot slot) {
    if (slot->parent->i2c_fd <= 0) {
        car_log_error("I2C device file never opened! Please call i2c_begin first.");
        return GPIO_ERR_I2C_ORDER;
    }

    if (close(slot->parent->i2c_fd) != 0) {
        car_log_error("I2c Device failed to close, this is probably bad.\n");
        return GPIO_ERR_IO_I2C;
    }
    return GPIO_SUCCESS;
}

u8 i2c_write_bytes(i2c_handle_slot slot, u8 reg, const u8 * buf, usize len) {
    //u8 reg = *buf;
    
    u8 rbuf[len + 1];
    rbuf[0] = reg;
    memcpy(rbuf + 1, buf, len);

    i2c_begin(slot);
    if (write(slot->parent->i2c_fd, buf, len) == -1) {
        car_log_error("Failed to write stuff to i2c device %d\n", slot->slave_address);
        return GPIO_ERR_IO_I2C;
    }
    i2c_end(slot);

    return GPIO_SUCCESS;
}

u8 i2c_read_bytes(i2c_handle_slot slot, u8 reg, u8 * buf, usize len) {

    u8 wbuf[1] = {reg};
    
    i2c_begin(slot);
    write(slot->parent->i2c_fd, wbuf, 1);
    if (read(slot->parent->i2c_fd, buf, len) != -1) {
        car_log_error("Failed to read bytes, either the write or read in this function failed.");
        return GPIO_ERR_IO_I2C;
    }

    i2c_end(slot);

    return GPIO_SUCCESS;
}

i2c_handle_slot i2c_get_device_handle(u8 address, gpio_i2c_state state) {
    i32 ind = 0;
    for (; ind < MAX_I2C_HANDLES && !state->i2c_handles[ind].is_free; ind++);
        

    if (ind == MAX_I2C_HANDLES) {
        car_log_error("No handles remaining for addr: %d, maybe give some back or smth.", address);
        return INVALID_HANDLE_VALUE;
    }

    i2c_handle_slot slot = &state->i2c_handles[ind];

    slot->is_free = FALSE;
    slot->slave_address = address;

    return slot;
}

void i2c_free_device_handle(i2c_handle_slot slot) {
   slot->is_free = TRUE;
}
