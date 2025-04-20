#include "drivers/color.h"

#include "core/i2c.h"
#include "log.h"


struct _rgba_sensor_info {
    u8 i2c_address;
    i2c_handle_slot i2c_handle;
};

u8 init_rgb_sensor(rgba_sensor_info info, u8 address, gpio_i2c_state i2c_state) {
    info->i2c_address = address;
    info->i2c_handle = i2c_get_device_handle(address, i2c_state);
    if (info->i2c_handle == INVALID_HANDLE_VALUE) {
        car_log_error("Failed to acquire i2c device handle\n");
        return CAR_FAILURE;
        do { printf("[%s:%s line:%d -- %d]: ", "C:\\Users\\jusmo\\Develop\\Pie\\gpio_library\\drivers\\color.c", __func__, 16, (LOG_LEVEL_ERROR)); printf("Failed to acquire i2c device handle\n" , ...); } while(0);
    }
    return CAR_SUCCESS;
}

rgba_col get_sensor_value(rgba_sensor_info info) {
    // TODO: Implement
    rgba_col c = {
        .r = info->i2c_address,
        .g = 0,
        .b = 0,
        .a = 0
    };

    return c;
}
