#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

#ifndef STREAM_SET_FILLER
#define STREAM_SET_FILLER 0x5a
#endif

extern struct marsh_test stream_set_test;

static uint32_t result;

/// Runs test.
static uint32_t stream_set_run(void)
{
    uint64_t cnt = 0;
    uint64_t *buffer = (uint64_t*)stream_set_test.heap;
    for(int i = 0; i < stream_set_test.iterations; i++) {
        buffer[i] = STREAM_SET_FILLER;
    }
    return buffer[0];
}


/// Prepares test.
static uint32_t stream_set_init(void)
{
    result = 0;
    marsh_get_x(&stream_set_test);
}


/// Validates test's result.
static uint32_t stream_set_verify(void)
{
    result = 0;
    uint64_t *buffer = (uint64_t*)stream_set_test.heap;
    for(int i = 0; i < stream_set_test.iterations; i++)
        result += (buffer[i] == STREAM_SET_FILLER) ? 0 : 1;
    return result;
}


/// Reports test's results.
static void stream_set_report(double elapsed, uint32_t retries)
{
    marsh_report(&stream_set_test, result, elapsed, retries);
}

MARSH_TEST(stream_set, stream_set_run, stream_set_init, stream_set_verify,
    stream_set_report);