
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

#ifndef GPIO_I2C_H
#define GPIO_I2C_H

#include "types.h"

typedef struct _gpio_i2c_state * gpio_i2c_state;
typedef struct _gpio_state * gpio_state;

typedef struct _i2c_handle_slot * i2c_handle_slot;


/**
 * @brief initializes i2c state (DO NOT CALL THIS DIRECTLY)
 *
 * This function shoudl not be called so i wil lnot docuent it
 *
 * @param state 
 * @return 
 */
u8 i2c_init(gpio_i2c_state * state);

// Begin writing/reading i2c bytes and stuff
u8 i2c_begin(i2c_handle_slot state);

// Stop writing/reading i2c bytes and stuff
u8 i2c_end(i2c_handle_slot state);


/**
 * @brief 
 * dont call this function I KNOW YOU WNATZ TO
 * @param state 
 * @return 
 */
u8 i2c_deinit(gpio_i2c_state * state);

/**
 * @brief Writes all the bytes in buf to the current i2c controller
 *
 * This function writes a buffer of bytes to the currently configured
 * i2c controller (slave), using len as the length of the buffer. 
 *
 * @param state initialized gpio state
 * @param buf buffer of bytes to write
 * @param len length of buf
 * @return GPIO_SUCCESS if successful
 */
u8 i2c_write_bytes(i2c_handle_slot slot, u8 reg, const u8 * buf, usize len);

/**
 * @brief  Reads all the bytes from i2c device into buf
 * 
 * @param state initialized gpio state
 * @param reg register to write to (this will be appened to buf)
 * @param buf buffer to write
 * @param len length of buffer
 * @return 
 */
u8 i2c_read_bytes(i2c_handle_slot slot, u8 reg, u8 * buf, usize len);


// Get an i2c handle which stores the active device address
// you want to use in any i2c operations
// honestly, this doesn't have a real need to exist,
// since the way I do the memory means that they rely on their 
// containing gpio_i2c_state's lifetime to exceed their own.
i2c_handle_slot i2c_get_device_handle(u8 address, gpio_i2c_state state);

void i2c_free_device_handle(i2c_handle_slot slot);

#endif
