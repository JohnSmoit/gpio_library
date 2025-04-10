#include "gpio.h"


int main() {
    gpio_state state;
    gpio_init(&state, GPIO_INIT_GPIO);

    gpio_deinit(&state);
    return 0;
}
