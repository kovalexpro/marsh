#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

#define STREAM_SET_FILLER 0x00

extern struct marsh_test stream_setz_test;

static uint32_t result;

/// Runs test.
static uint32_t stream_setz_run(void)
{
    uint64_t cnt = 0;
    uint64_t *buffer = (uint64_t*)stream_setz_test.heap;
    for(int i = 0; i < stream_setz_test.iterations; i++) {
        buffer[i] = STREAM_SET_FILLER;
    }
    return buffer[0];
}


/// Prepares test.
static uint32_t stream_setz_init(void)
{
    result = 0;
    marsh_get_x(&stream_setz_test);
}


/// Validates test's result.
static uint32_t stream_setz_verify(void)
{
    result = 0;
    uint64_t *buffer = (uint64_t*)stream_setz_test.heap;
    for(int i = 0; i < stream_setz_test.iterations; i++)
        result += (buffer[i] == STREAM_SET_FILLER) ? 0 : 1;
    return result;
}


/// Reports test's results.
static void stream_setz_report(double elapsed, uint32_t retries)
{
    marsh_report(&stream_setz_test, result, elapsed, retries);
}

MARSH_TEST(stream_setz, stream_setz_run, stream_setz_init, stream_setz_verify,
    stream_setz_report);
