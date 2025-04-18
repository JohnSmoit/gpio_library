#ifndef GPIO_ASYNC_H
#define GPIO_ASYNC_H

#include "types.h"

typedef struct _gpio_thread_pool _gpio_thread_pool;
typedef _gpio_thread_pool * gpio_thread_pool;

typedef struct _gpio_async_engine _gpio_async_engine;
typedef _gpio_async_engine * gpio_async_engine;

typedef void * (* gpio_async_provider_impl)(void *);

enum gpio_async_status_code {
    ASYNC_SUCCESS,
};

enum gpio_async_signal {
    FUCKING_STOP,
    STOP,
    PLEASE_STOP,
};

enum gpio_validation_mode {
    VALIDATION_LEVEL_FATAL,
};

typedef gpio_async_status_code (* gpio_async_handler_impl)(void *);

void gpio_create_thread_pool(gpio_thread_pool pool, u8 thread_count);

// stub for now, need to work out memory layout for error/validation logic
void gpio_validate(void * thingy, gpio_validation_mode mode);

void gpio_create_async_engine(gpio_async_engine engine, gpio_thread_pool pool);

void gpio_async_engine_start(gpio_async_engine engine);

void gpio_bind_async_provider(
    gpio_async_engine engine, 
    gpio_async_provider_impl callback, 
    const char * provider_name
);

// requesters

// if you see this comment this is a sign that you should not be using this function
// which to be honest you should have gotten by the name. I mean, seriously? 
// did the horrifying name not tip you off? NO USE THIS PRETTY PLEASE.
void * ____no_use____gpio_sync_request_but_bad(gpio_async_engine engine, const char * name);
#define gpio_sync_request(engine, name, type) *(type*)___no_use___gpio_sync_request_but_bad(engine, name)



void gpio_async_request(
    gpio_async_engine engine, 
    const char * name,
    gpio_async_handler_impl handler
);

void gpio_async_global_signal(gpio_async_engine engine, gpio_async_signal signal);

void gpio_destroy_async_engine(gpio_async_engine engine);

#endif
