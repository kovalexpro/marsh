
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define joincat(F,D) F##_##D
#define join(F,D) joincat(F,D)

#define thisname join(MARSH_TEST_NAME, datatype)
#define thisfunc(f) join(thisname, f)
#define thistest thisfunc(test)

enum {
    MARSH_ALLOC_ALIGN = 16,
    MARSH_ALLOC_STRIDE = 8,
    MARSH_WARMUP_RETRY = 1000,
    MARSH_RETRY = 1000000,
    MARSH_HEAP_SIZE = 128 * 1024 * 1024,
    MARSH_L2C_SIZE = 256 * 1024,
};

#ifndef MARSH_ITERATIONS
#define MARSH_ITERATIONS (MARSH_L2C_SIZE / sizeof(uint64_t))
#endif

#ifndef BUILD_VARIANT
#define BUILD_VARIANT "unspecified"
#endif

#ifndef BUILD_REV
#define BUILD_REV "unspecified"
#endif

#ifndef BUILD_BRANCH
#define BUILD_BRANCH "unspecified"
#endif

#ifndef BUILD_DATE
#define BUILD_DATE __TIMESTAMP__
#endif

/// Current test.
extern struct marsh_test *test;

/// Timing value's type.
typedef clock_t marsh_time_t;

/// Test desriptor.
struct marsh_test {
    uint64_t read_size;
    uint64_t write_size;
    uint32_t iterations;
    const char* name;
    void* heap;
    void *r, *x, *y, *z;
    void* private;
    uint32_t (*run)(void);
    uint32_t (*init)(void);
    uint32_t (*verify)(void);
    void (*report)(double elapsed, uint32_t retries);
};

/// Macro for creating test descriptor structure.
#define MARSH_TEST(test_name, test_run, test_init, test_verify, test_report) \
    struct marsh_test test_name##_test = { \
        .read_size = 0, \
        .write_size = 0, \
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

#define THIS_MARSH_TEST(test_name, test_run, test_init, test_verify, test_report) \
    MARSH_TEST(test_name, test_run, test_init, test_verify, test_report)

void* marsh_alloc(uint32_t size);

void marsh_free(void* p);

void marsh_random_data(void* p, uint32_t size);

marsh_time_t marsh_time();

double marsh_elapsed(marsh_time_t start, marsh_time_t stop);

void marsh_report(struct marsh_test *test, uint32_t errors,
    double elapsed, uint32_t retries, double mark);

void marsh_get_r(struct marsh_test *test, uint32_t elem_size);

void marsh_get_x(struct marsh_test *test, uint32_t elem_size);

void marsh_get_xy(struct marsh_test *test, uint32_t elem_size);

void marsh_get_xyz(struct marsh_test *test, uint32_t elem_size);
