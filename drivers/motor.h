#ifndef GPIO_MOTOR_H
#define GPIO_MOTOR_H

#include "types.h"

struct _motor_control_info;

typedef struct _motor_control_info * motor_control_info;

u8 motor_init(motor_control_info control, u8 address, u8 motor);

u8 motor_set_speed(motor_control_info control, u8 direction, u8 speed_percent);

#endif
