#include "core/uart.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "log.h"


struct _gpio_uart_handle {
    i32 uart_fd;
    // other stuff probs
};

struct _gpio_uart_state {
    struct _gpio_uart_handle handel;
}; 


u32 gpio_init_uart(gpio_uart_state state) {
    state->handel.uart_fd = -1;

    return CAR_SUCCESS;
}

u32 gpio_open_uart_device(gpio_uart_state state, u32 baud_rate) {
    struct termios params = {0};


    if ((state->handel.uart_fd = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY))) {
        car_log_error("Could not open the device file for the uart port uh oh!\n"); 
        return CAR_FAILURE;
    }

    if (tcgetattr(state->handel.uart_fd, &params) < 0) {
        car_log_error("Error getting terminal attributes whihc is a bad thing\n");
        close(state->handel.uart_fd);

        return CAR_FAILURE;
    }

    // configure the terminal output for uart n stuff
    params.c_cflag = CS8 | CLOCAL | CREAD;
    params.c_iflag = IGNPAR;
    params.c_oflag = 0;
    params.c_lflag = 0;

    cfsetispeed(&params, baud_rate);
    cfsetospeed(&params, baud_rate);

    params.c_cc[VMIN] = 1;
    params.c_cc[VTIME] = 0;
    
    if (tcsetattr(state->handel.uart_fd, TCSANOW, &params) < 0) {
        car_log_error("Error setting configuration terminal attributes aaaaaaaaaaaag.\n");
        close(state->handel.uart_fd);
        
        return CAR_FAILURE;
    }

    return CAR_SUCCESS;
}

void gpio_close_uart_device(gpio_uart_state state, gpio_uart_handle handle) {
    
}

u32 gpio_uart_write(gpio_uart_handle handle, const u8 * buf, u32 count) {
    
}

u32 gpio_uart_read(gpio_uart_handle handle, const u8 * buf, u32 count) {
    
}
