#include "drivers/motor.h"

#include <stdlib.h>

#include "log.h"
#include "types.h"

#include "core/i2c.h"

struct _motor_control_info {
    i2c_handle_slot i2c_handle;
    u8 motor_slot;
};

u8 motor_init(motor_control_info * control, gpio_i2c_state state, u8 address, u8 motor) {
    motor_control_info c = (motor_control_info)malloc(sizeof(struct _motor_control_info));

    c->i2c_handle = i2c_get_device_handle(address, state);
    c->motor_slot = motor;

    *control = c;

    return CAR_SUCCESS;
}

u8 motor_set_speed(motor_control_info control, u8 direction, u8 speed_percent) {
    car_log_info("POOTIS MOTOR SET: dir %d spd %d\n", direction, speed_percent);

    return control != INVALID_HANDLE_VALUE;
}

u8 motor_deinit(motor_control_info * control) {
    if (!control || !*control) {
        car_log_info("No control detected lol\n");
        return CAR_SUCCESS;
    }
    motor_control_info c = *control;
    motor_set_speed(c, 0, 0);
    i2c_free_device_handle(c->i2c_handle);
    
    free(c);
    *control = NULL;
    
    return CAR_SUCCESS;
}
