#include "types.h"

// handle to the state required to run an rgba sensor
// read loop
typedef struct _rgba_sensor_info* rgba_sensor_info;

// structure representing rgb color values
// each value is an 8-bit unsigned integer value.
typedef struct rgba_col {
    u8 r; // red
    u8 g; // green
    u8 b; // blue
    u8 a; // alpha
} rgba_col;

u8 init_rgb_sensor(rgba_sensor_info info);
rgba_col get_sensor_value(rgba_sensor_info info);

