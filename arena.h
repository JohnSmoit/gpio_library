#ifndef GPIO_ARENA_H
#define GPIO_ARENA_H

#include "types.h"

typedef uint32_t gpio_lifetime_handle;

typedef struct gpio_arena gpio_arena;
typedef struct gpio_arena * gpio_arena_handle;

gpio_lifetime_handle arena_create_lifetime(gpio_arena_handle arena);

void arena_free_lifetime(gpio_arena_handle arena, gpio_lifetime_handle handle);

void * arena_allocate(gpio_arena_handle arena, gpio_lifetime_handle handle, usize count);
#define arena_allocate_t(arena, handle, type, count) arena_allocate(arena, handle, sizeof(type) * (count)) 

#endif
