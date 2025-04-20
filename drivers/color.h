#include "types.h"

typedef struct _gpio_i2c_state * gpio_i2c_state;

// handle to the state required to run an rgba sensor
// read loop
typedef struct _rgba_sensor_info * rgba_sensor_info;


// structure representing rgb color values
// each value is an 8-bit unsigned integer value.
typedef struct rgba_col {
    float r; // red
    float g; // green
    float b; // blue
    float a; // alpha
} rgba_col;

// initialize an rgb sensor with its relevant state information
// additionally, probably needs to know which i2c device to use...
u8 init_rgb_sensor(rgba_sensor_info info, u8 address, gpio_i2c_state i2c_state);

// get the rgb sensor's output values in an rgba color format
rgba_col get_sensor_value(rgba_sensor_info info);

