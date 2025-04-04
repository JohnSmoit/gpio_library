
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

#ifndef GPIO_I2C_H
#define GPIO_I2C_H

#include "types.h"

typedef struct gpio_i2c_state gpio_i2c_state;
typedef struct gpio_state gpio_state;

int i2c_init(gpio_state * state);

int i2c_deinit(gpio_state * state);

int i2c_write_bytes(gpio_state * state, const u8 * buf, usize len);

int i2c_read_bytes(gpio_state * state, u8 reg, u8 * buf, usize len);

int i2c_init_slave_address(gpio_state * state, u8 addr);


#endif
