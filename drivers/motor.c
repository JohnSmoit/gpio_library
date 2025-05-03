#include "drivers/motor.h"

#include <stdlib.h>

#include "log.h"
#include "types.h"

#include "core/i2c.h"

struct _motor_control_info {
    i2c_handle_slot i2c_handle;
    u8 motor_slot;
};

#define PWM_CTL_A PCA_CHANNEL_0
#define MOTOR_IN_A PCA_CHANNEL_1
#define MOTOR_OUT_A PCA_CHANNEL_2

#define PWM_CTL_B PCA_CHANNEL_3
#define MOTOR_IN_B PCA_CHANNEL_4
#define MOTOR_OUT_B PCA_CHANNEL_5


#define MOTOR_DIR_1 0
#define MOTOR_DIR_2 1

#define BUTTON_PIN 17

void set_pwm(i2c_handle_slot slot, u8 channel, u16 on, u16 off) {
    u8 reg1 = PCA_REG(channel, PCA_REG_MODE_ON, PCA_REG_PART_LO);
    u8 reg2 = PCA_REG(channel, PCA_REG_MODE_ON, PCA_REG_PART_HI);
    u8 reg3 = PCA_REG(channel, PCA_REG_MODE_OFF, PCA_REG_PART_LO);
    u8 reg4 = PCA_REG(channel, PCA_REG_MODE_OFF, PCA_REG_PART_HI);

    // TODO: Buffer I2C Write calls
    i2c_write_single(slot, reg1, on & 0xff);
    i2c_write_single(slot, reg2, on >> 8);
    i2c_write_single(slot, reg3, off & 0xff);
    i2c_write_single(slot, reg4, off >> 8);
}
void set_motor_lvl(i2c_handle_slot slot, int speed, int direction, u8 motor) {
    int adj_spd = (int)(speed * (4096 / 100.0f) - 1);
    car_log_info("Speed: %d\n", adj_spd);

    int motor_1 = 0;
    int motor_2 = 0;
    if (direction == MOTOR_DIR_1) {
        motor_1 = 4095;
        motor_2 = 0;
    } else {
        motor_1 = 0;
        motor_2 = 4095;
    }

    u8 c1 = 0;
    u8 c2 = 0;
    u8 c3 = 0;
    if (motor == MOTOR_SLOT_A) {
        c1 = PCA_CHANNEL_0;
        c2 = PCA_CHANNEL_1;
        c3 = PCA_CHANNEL_2;
    } else {
        c1 = PCA_CHANNEL_5;
        c2 = PCA_CHANNEL_3;
        c3 = PCA_CHANNEL_4;
    }

    set_pwm(slot, c1, 0, adj_spd);
    set_pwm(slot, c2, 0, motor_1);
    set_pwm(slot, c3, 0, motor_2);
}

u8 motor_init(motor_control_info * control, gpio_i2c_state state, u8 address, u8 motor) {
    motor_control_info c = (motor_control_info)malloc(sizeof(struct _motor_control_info));
    
    car_log_info("State pointer address: %p\n", state);
    c->i2c_handle = i2c_get_device_handle(address, state);
    c->motor_slot = motor;

    *control = c;

    i2c_write_single(c->i2c_handle, 0, 0);

    return CAR_SUCCESS;
}

u8 motor_set_speed(motor_control_info control, u8 direction, u8 speed_percent) {
    if (control == INVALID_HANDLE_VALUE) {
        car_log_error("Invalid motor control handle!");
        return CAR_FAILURE;
    }
    car_log_info("POOTIS MOTOR SET: adr: %d dir %d spd %d\n", i2c_get_address(control->i2c_handle) , direction, speed_percent);

    set_motor_lvl(control->i2c_handle, speed_percent, direction, control->motor_slot);

    return CAR_SUCCESS;
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
