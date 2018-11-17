
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

enum {
    MARSH_WARMUP_RETRY = 1000,
    MARSH_RETRY = 1000000,
    MARSH_HEAP_SIZE = 128 * 1024 * 1024,
    MARSH_L2C_SIZE = 256 * 1024,
};

#ifndef MARSH_ITERATIONS
#define MARSH_ITERATIONS (MARSH_L2C_SIZE / sizeof(uint64_t))
#endif

/// Current test.
extern struct marsh_test *test;

/// Timing value's type.
typedef clock_t marsh_time_t;

/// Test desriptor.
struct marsh_test {
    uint32_t iterations;
    const char* name;
    void* heap;
    void* private;
    uint32_t (*run)(void);
    uint32_t (*init)(void);
    uint32_t (*verify)(void);
    void (*report)(double elapsed, uint32_t retries);
};

/// Macro for creating test descriptor structure.
#define MARSH_TEST(test_name, test_run, test_init, test_verify, test_report) \
    struct marsh_test test_name##_test = { \
        .iterations = MARSH_ITERATIONS, \
        .name = #test_name, \
        .heap = 0, \
        .private = 0, \
        .run = test_run, \
        .init = test_init, \
        .verify = test_verify, \
        .report = test_report \
    }; \
    struct marsh_test *test = &test_name##_test;


void* marsh_alloc(uint32_t size);

void marsh_free(void* p);

void marsh_random_data(void* p, uint32_t size);

marsh_time_t marsh_time();

double marsh_elapsed(marsh_time_t start, marsh_time_t stop);
