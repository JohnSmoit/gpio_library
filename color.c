#include <stdio.h>


typedef struct color {
    float r;
    float g;
    float b;
} color;

typedef struct confidence_results {
    float c_dot; // relative color value difference
    float c_mag; // relative magnitude diffreence
} confidence_results;

color norm(color a) {
    
}

float mag(color a) {
    
}

// normalize the color
float dot(color a, color b) {

}



confidence_results color_confidence(color a, color b) {
    return {0, 0};
}

const char * get_color_name(color a) {
    return "NONE";
}

void print_thing(color a, color a1) {
    confidence_results con = color_confidence(a, a1);
    const char * name = get_color_name(a1);
    printf("Comparing color {r:%f, g:%f, b:%f} to {r:%f, g:%f, b:%f}\n", a.r, a.g, a.b, a1.r, a1.g, a1.b);
    printf("Confidence: %02f mag_diff: %02f, %s\n", con.c_dot, con.c_mag, name);
}

int main() {
    color a = { .r = 100.0, .g = 10.0, .b = 10.0};
    color a1 = { .r = 100.0, .g = 10.0, .b = 10.0};
    print_thing(a, a1);

    color b = { .r = 100.0, .g = 150.0, .b = 10.0};
    color b1 = { .r = 20, .g = 30, .b = 100};
    print_thing(b, b1);

    color c = { .r = 255.0, .g = 255.0, .b = 255.0};
    color c1 = {.r = 1, .g = 1, .b = 1};
    print_thing(c, c1);

    color d = { .r = 255, .g = 1, .b = 1};
    color d1 = { .r = 255, .g = 10, .b = 1};
    print_thing(d, d1);

    return 0;
}
