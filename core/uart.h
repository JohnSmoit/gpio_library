#ifndef GPIO_UART_H
#define GPIO_UART_H

#include "types.h"

typedef struct _gpio_uart_state * gpio_uart_state;

typedef struct _gpio_uart_handle * gpio_uart_handle;


u32 gpio_init_uart(gpio_uart_state state);

u32 gpio_open_uart_device(gpio_uart_state state, u32 baud_rate);

void gpio_close_uart_device(gpio_uart_state state, gpio_uart_handle handle);

u32 gpio_uart_write(gpio_uart_handle handle, const u8 * buf, u32 count);

u32 gpio_uart_read(gpio_uart_handle handle, const u8 * buf, u32 count);


#endif
