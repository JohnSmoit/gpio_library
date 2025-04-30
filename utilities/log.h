#ifndef CAR_LOG_H
#define CAR_LOG_H

#include <stdio.h>
#include "types.h"

typedef enum car_log_levels {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
} car_log_levels;

#define CAR_DEBUG

void do_stacktrace(int log_level);

// this is a very disgusting macro i almost used
// leaving it here just for reference
#define VA_NARGS(...) \
    ((u32)(sizeof((u32[]){__VA_ARGS__})/sizeof(u32)))

#define VA_EXPAND(...) __VA_OPT__(,) __VA_ARGS__

#define car_log(fmt, log_level, ...) \
    car_log_impl(fmt, (log_level) __VA_OPT__(,) __VA_ARGS__)

#define car_log_info(fmt, ...) \
    car_log(fmt, LOG_LEVEL_INFO __VA_OPT__(,) __VA_ARGS__)

#define car_log_warn(fmt, ...) \
    car_log(fmt, LOG_LEVEL_WARN __VA_OPT__(,) __VA_ARGS__)

#define car_log_error(fmt, ...) \
    car_log(fmt, LOG_LEVEL_ERROR __VA_OPT__(,) __VA_ARGS__)

#define car_log_fatal(fmt, ...) \
    car_log(fmt, LOG_LEVEL_FATAL __VA_OPT__(,) __VA_ARGS__)

#if defined(CAR_DEBUG)
#define car_log_impl(fmt, log_level, ...) \
do { \
    printf("[%s:%s line:%d -- %d]: ", __FILE__, __func__, __LINE__, log_level); \
    printf(fmt __VA_OPT__(,) __VA_ARGS__); \
    do_stacktrace(log_level); \
       } while(0)
#else
    #define car_log_impl(fmt, log_level, ...) 
#endif


#endif
