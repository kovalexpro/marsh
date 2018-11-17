#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test popcnt_test;

const static uint32_t popcnt_golden = 0x1f018;

static uint32_t result;

/// Runs test.
static uint32_t popcnt_run(void)
{
    uint64_t cnt = 0;
    uint64_t *buffer = (uint64_t*)popcnt_test.heap;
    for(int i = 0; i < popcnt_test.iterations; i++) {
        cnt += __builtin_popcountll(buffer[i]);
    }
    result = (uint32_t)(cnt & 0xfffffff) ^ (uint32_t)(cnt >> 32);
    return result;
}


/// Prepares test.
static uint32_t popcnt_init(void)
{
    result = 0;
    popcnt_test.heap = marsh_alloc(popcnt_test.iterations * sizeof(uint64_t));
    uint64_t *buffer = (uint64_t*)popcnt_test.heap;
    marsh_random_data(buffer, popcnt_test.iterations);
}


/// Validates test's result.
static uint32_t popcnt_verify(void)
{
    return result == popcnt_golden ? 0 : 1;
}


/// Reports test's results.
static void popcnt_report(double elapsed, uint32_t retries)
{
    if (result != popcnt_golden)
        printf("Result: %8.8x Expected: %8.8x\n", result, popcnt_golden);
    printf("iterations=%d, data=%.3fKB, bandwidth: %.3f GB/s\n",
        popcnt_test.iterations,
        popcnt_test.iterations * sizeof(uint64_t) / 1e3,
        popcnt_test.iterations * sizeof(uint64_t) * retries / elapsed / 1e9);
}

MARSH_TEST(popcnt, popcnt_run, popcnt_init, popcnt_verify, popcnt_report);
