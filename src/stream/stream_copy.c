#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test stream_copy_test;

static uint32_t result;

/// Runs test.
static uint32_t stream_copy_run(void)
{
    uint64_t cnt = 0;
    uint32_t n = stream_copy_test.iterations;
    uint64_t *to_buf = (uint64_t*)stream_copy_test.heap ;
    uint64_t *from_buf = (uint64_t*)stream_copy_test.heap + n;
    for(int i = 0; i < n; i++) {
        to_buf[i] = from_buf[i];
    }
    return (uint32_t)to_buf[0];
}


/// Prepares test.
static uint32_t stream_copy_init(void)
{
    result = 0;
    marsh_get_r(&stream_copy_test, sizeof(uint64_t));
    marsh_get_x(&stream_copy_test, sizeof(uint64_t));
}


/// Validates test's result.
static uint32_t stream_copy_verify(void)
{
    result = 0;
    uint32_t n = stream_copy_test.iterations;
    uint64_t *to_buf = (uint64_t*)stream_copy_test.heap ;
    uint64_t *from_buf = (uint64_t*)stream_copy_test.heap + n;
    for(int i = 0; i < stream_copy_test.iterations; i++)
        result += (to_buf[i] == from_buf[i]) ? 0 : 1;
    return result;
}


/// Reports test's results.
static void stream_copy_report(double elapsed, uint32_t retries)
{
    double mark = test->write_size * retries / elapsed / 1e9;
    marsh_report(&stream_copy_test, result, elapsed, retries, mark);
}

MARSH_TEST(stream_copy, stream_copy_run, stream_copy_init,
    stream_copy_verify, stream_copy_report);
