#include "types.h"

typedef struct rgba_sensor_info rgba_sensor_info;
typedef struct rgba_col rgba_col;

rgba_sensor_info init_rgb_sensor(u8 address);
rgba_col get_sensor_value(rgba_sensor_info info);

