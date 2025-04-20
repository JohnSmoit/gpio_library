#include "drivers/color.h"

#include <math.h>

#include "core/i2c.h"
#include "log.h"
#include "core/gpio.h"

#define TCS34725_COMMAND_BIT 0x80
#define TCS34725_ADATAL (0x14) /**< Clear channel data low byte */
#define TCS34725_RDATAL (0x16) /**< Red channel data low byte */
#define TCS34725_GDATAL (0x18) /**< Green channel data low byte */
#define TCS34725_BDATAL (0x1A) /**< Blue channel data low byte */

struct _rgba_sensor_info {
    u8 i2c_address;
    i2c_handle_slot i2c_handle;
};


#define TABLE_SIZE 256
const float gamma_val = 2.5f;


float gamma_table[TABLE_SIZE] = {0};

// here we initialize the gamma table for every possible byte value
// this isn't strictly neccesary but it helps speed things up on slower machines
void init_gamma_table() {
    for (u32 i = 0; i < TABLE_SIZE; i++) {
        gamma_table[i] = pow((float) i / TABLE_SIZE, gamma_val) * 255.0f + 0.5f;
    }
}

u8 init_rgb_sensor(rgba_sensor_info info, u8 address, gpio_i2c_state i2c_state) {
    info->i2c_address = address;
    info->i2c_handle = i2c_get_device_handle(address, i2c_state);
    if (info->i2c_handle == INVALID_HANDLE_VALUE) {
        car_log_error("Failed to acquire i2c device handle\n");
        return CAR_FAILURE;

    }
    if (gamma_table[TABLE_SIZE - 1] == 0) {
        init_gamma_table();
    }

    return CAR_SUCCESS;
}

float clampf(float a, float min, float max) {
    const float temp = a < min ? min : a;
    return temp > max ? max : temp; 
}

rgba_col color_correct(rgba_col color) {   
    // start with gamma correction, this adjusts the color to better match the human
    // eye's color range from the sensor. but this isn't enough...
    rgba_col adj = {
        .r = gamma_table[(int)color.r],
        .g = gamma_table[(int)color.g],
        .b = gamma_table[(int)color.b]
    };
    
    // to account for the weird color value output (I think it's the chromaticty),
    // I apply this ungodly transformation to boost the interesting color values 
    // to coerce a better output from the color detection code.

    // this is a custom transformation that should hopefully 
    // increase contrast in a way that doesn't destroy the overall brightness
    float max = fmax(adj.r, fmax(adj.g, adj.b));
    float min = fmin(adj.r, fmin(adj.g, adj.b));
    //printf("max: %f, min: %f, {r:%f, g:%f, b:%f}\n", max, min, adj.r, adj.g, adj.b);

    float diff = fmax(1, max - min);
    //printf("Range Diff: %f\n", diff);

    float himult = 1 + diff / gamma_table[TABLE_SIZE - 1];
    float lomult = 1 - diff / gamma_table[TABLE_SIZE - 1];
    float mult_range = himult - lomult;
    //printf("himult: %f\nlomult: %f\nmult_range:%f\n", himult, lomult, mult_range);

    // get the relative color range of each component within the total span of the color
    float rd = lomult + mult_range * (1 - (diff - (adj.r - min)) / diff);
    float gd = lomult + mult_range * (1 - (diff - (adj.g - min)) / diff);
    float bd = lomult + mult_range * (1 - (diff - (adj.b - min)) / diff);

    //printf("rd:%f\ngd:%f\nbd:%f\n", rd, gd, bd);

    adj.r = clampf(adj.r * rd, 0, 255.0);
    adj.g = clampf(adj.g * gd, 0, 255.0);
    adj.b = clampf(adj.b * bd, 0, 255.0);


    return adj;

}

u16 read_single_rgb_channel(u8 reg, rgba_sensor_info info) {
    u8 cmd = TCS34725_COMMAND_BIT | reg;
    u16 buf;

    if (i2c_read_bytes(info->i2c_handle, cmd, (u8 *)&buf, 2) != GPIO_SUCCESS) {
    car_log_error("Failed to read rgb channel %d.", reg);
    return 0;
}
    // Combine as little-endian return ((buf[1] << 8) | buf[0]);
    return (((buf & (0xFF << 8))) | (buf & 0xFF));
}

rgba_col get_sensor_value(rgba_sensor_info info) {
    u16 r = read_single_rgb_channel(TCS34725_RDATAL, info);
    u16 b = read_single_rgb_channel(TCS34725_BDATAL, info);
    u16 g = read_single_rgb_channel(TCS34725_GDATAL, info);
    u16 a = read_single_rgb_channel(TCS34725_ADATAL, info);
    
    float red = r;
    float green = g;
    float blue = b;
    float alpha = a;

    float rnorm = red / alpha;
    float gnorm = green / alpha;
    float bnorm = blue / alpha;

    
    if (alpha == 0) {
        rgba_col col = {0};
        return col;
    }
    r = (u16)(rnorm * 255.0);
    g = (u16)(gnorm * 255.0);
    b = (u16)(bnorm * 255.0);
    
    rgba_col col = {
        .r = r,
        .g = g,
        .b = b,
        .a = a,
    };
    
    return color_correct(col);
}
