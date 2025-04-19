
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

typedef struct gpio_i2c_state gpio_i2c_state;
typedef struct gpio_state gpio_state;


/**
 * @brief initializes i2c state (DO NOT CALL THIS DIRECTLY)
 *
 * This function shoudl not be called so i wil lnot docuent it
 *
 * @param state 
 * @return 
 */
int i2c_init(gpio_state * state);

/**
 * @brief 
 * dont call this function I KNOW YOU WNATZ TO
 * @param state 
 * @return 
 */
int i2c_deinit(gpio_state * state);

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
int i2c_write_bytes(gpio_state * state, u8 reg, const u8 * buf, usize len);

/**
 * @brief  Reads all the bytes from i2c device into buf
 * 
 * @param state initialized gpio state
 * @param reg register to write to (this will be appened to buf)
 * @param buf buffer to write
 * @param len length of buffer
 * @return 
 */
int i2c_read_bytes(gpio_state * state, u8 reg, u8 * buf, usize len);

/**
 * @brief set the currently active slave address (or controller)
 *
 * @param state initailized gpio state
 * @param addr address to set
 * @return GPIO_SUCCESS
 */
int i2c_init_slave_address(gpio_state * state, u8 addr);


#endif
