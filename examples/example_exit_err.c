#include "gpio.h"



int main() {
    gpio_state state;
    
    // exit_err will exit with an error message if
    // the function being nested into it returns a non
    // GPIO_SUCCESS (0) value
    //                        vvv This function
    gpio_exit_err(&state, gpio_init(&state, GPIO_INIT_GPIO));
}
