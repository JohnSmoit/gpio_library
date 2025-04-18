#include "../async.h"

#define ASYNC_THREADS_COUNT 4

gpio_async_status_code provide_some_shit(void * params, void * inp) {
    poopy_structure * res = (poopy_structure *)inp;

    *res = {
        .a = 100,
        .b = 200,
        .stroongalagoongala = "stroongalagoongala",
    };

    return  
}

void * provide_some_shit_slowly(void * params) {
    
}


int poopy_handler(void * params) {
   
}

int slow_poopy_handler(void * params) {
    
}

typedef struct poopy_structure {
    int a;
    int b;
    char * stroongalagoongala;
} poopy_structure;

typedef struct slow_poopy_structure {
    int time;
    int time_plus_one;
    int time_plus_two;
    int time_subtracted_by_time_minus_one;
} slow_poopy_structure;

int main() {
    // thread pooled async
    gpio_thread_pool async_threads;
    gpio_async_engine async_engine;

    gpio_create_thread_pool(async_threads, ASYNC_THREADS_COUNT);
    gpio_validate(async_threads, VALIDATION_LEVEL_FATAL);

    gpio_create_async_engine(async_engine, async_threads);
    gpio_validate(async_threads, VALIDATION_LEVEL_FATAL);

    gpio_async_engine_start(async_engine);

    // runtime async routine binding (to event loop)
    gpio_bind_async_provider(async_engine, provide_some_shit, "speedy");
    gpio_bind_async_provider(async_engine, provide_some_shit_slowly, "slowass");
    
    // this is actually a MARCO
    poopy_structure result = gpio_sync_request(async_engine, "speedy", poopy_structure);
    
    // this is actually a MACKRO
    slow_poopy_structure slow_result = gpio_sync_request(async_engine, "slowass", slow_poopy_structure);

    gpio_async_request(async_engine, "speedy", poopy_handler);
    gpio_async_request(async_engine, "slowass", slow_poopy_handler);

    gpio_async_global_signal(async_engine, FUCKING_STOP);
    gpio_async_join(async_engine);

    gpio_destroy_thread_pool(async_threads);
    gpio_destroy_async_engine(async_engine);

    // single-thread async (blocking)
    gpio_async_engine blocking_engine;
    gpio_create_async_engine(blocking_engine, NULL);

    gpio_bind_async_provider(blocking_engine, provide_some_shit, "speedy2");
    gpio_validate(async_threads, VALIDATION_LEVEL_FATAL);

    gpio_bind_async_provider(blocking_engine, provide_some_shit_slowly, "slowass2");
    gpio_validate(async_threads, VALIDATION_LEVEL_FATAL);

    // if the engine has no thread pool the control over event loop 
    // execution is given to the main thread.
    // also async requests block the main thread via a spinlock or smth
    gpio_async_engine_start(blocking_engine);

    result = gpio_sync_request(blocking_engine, "speedy2", poopy_structure);

    gpio_async_request(blocking_engine, "slowass2", slow_poopy_handler);

    gpio_async_global_signal(blocking_engine, FUCKING_STOP);
    
    // no need to join since this async engine is all on the main thread.
    gpio_destroy_async_engine(blocking_engine);

    return 0;
}
