#ifndef GPIO_MOTOR_H
#define GPIO_MOTOR_H

#include "types.h"

struct _motor_control_info;

typedef struct _motor_control_info * motor_control_info;
typedef struct _gpio_i2c_state * gpio_i2c_state;

#define MOTOR_SLOT_A PCA_CHANNEL_0
#define MOTOR_SLOT_B PCA_CHANNEL_3

u8 motor_init(motor_control_info * control, gpio_i2c_state state, u8 address, u8 motor);

u8 motor_set_speed(motor_control_info control, u8 direction, u8 speed_percent);

// TODO: better handling then just a malloc
u8 motor_deinit(motor_control_info * control);

#endif
