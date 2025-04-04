
/***************************************************************
* Class:: CSC-615-01 Spring 2024
* Name:: Ryan Algazi
* Student ID:: 921914536
* Github-Name:: JohnSmoit
* Project:: Assignment 2 - Tapeless Ruler
*
* File:: types.h
*
* Description::
*   Defines some type aliases that I like to use, 
*   sorry if they look gross.
**************************************************************/

#ifndef GPIO_TYPES_H
#define GPIO_TYPES_H

#include <stdint.h>

// standard bit width types (mostly)
// renamed for my convenience
typedef unsigned long long uflags;
typedef unsigned long usize;

typedef uint8_t u8;
typedef int8_t i8;

typedef uint16_t u16;
typedef int16_t i16;

typedef uint32_t u32;
typedef int32_t i32;

typedef uint64_t u64;
typedef int64_t i64;

typedef uint8_t bool;

// TODO: Partition GPIO statuses into 2 16-bit sections
// and reserve the additional 16 bits for more specific information
// (which function failed, underlying reason, etc)
enum gpio_statuses {
    GPIO_SUCCESS,
    GPIO_TIMEOUT,
    GPIO_ERR_MMAP,
    GPIO_ERR_IO_I2C,
    GPIO_ERR_MEMORY,
};

enum gpio_init_flags {
    GPIO_INIT_GPIO = 1,
    GPIO_INIT_I2C = 2,
};


#define GPIO_INIT_COUNT 2

#define PCA_REG_PART_LO 0
#define PCA_REG_PART_HI 1

#define PCA_REG_MODE_ON 0
#define PCA_REG_MODE_OFF 2

#define PCA_BASE_REG 0x06

#define PCA_REG(channel, mode, part) \
    PCA_BASE_REG + ((mode) + (part)) + 4 * (channel) 

enum pca_channel {
    PCA_CHANNEL_0,
    PCA_CHANNEL_1,
    PCA_CHANNEL_2,
    PCA_CHANNEL_3,
    PCA_CHANNEL_4,
    PCA_CHANNEL_5,
    PCA_CHANNEL_6,
    PCA_CHANNEL_7,
    PCA_CHANNEL_8,
    PCA_CHANNEL_9,
    PCA_CHANNEL_10,
    PCA_CHANNEL_11,
    PCA_CHANNEL_12,
    PCA_CHANNEL_13,
    PCA_CHANNEL_14,
    PCA_CHANNEL_15,
};

#endif
