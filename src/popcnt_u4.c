#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test popcnt_u4_test;

const static uint32_t popcnt_u4_golden = 0x1f018;

static uint32_t result;

/// Runs test.
static uint32_t popcnt_u4_run(void)
{
    uint64_t *buffer = (uint64_t*)popcnt_u4_test.heap;
    uint64_t cnt0, cnt1, cnt2, cnt3;
    cnt0 = cnt1 = cnt2 = cnt3 = 0;
    for(size_t i = 0; i < popcnt_u4_test.iterations; i+=4) {
        cnt0 += __builtin_popcountll(buffer[i+0]);
        cnt1 += __builtin_popcountll(buffer[i+1]);
        cnt2 += __builtin_popcountll(buffer[i+2]);
        cnt3 += __builtin_popcountll(buffer[i+3]);
    }
    uint64_t cnt = cnt0 + cnt1 + cnt2 + cnt3;
    result = (uint32_t)(cnt & 0xfffffff) ^ (uint32_t)(cnt >> 32);
    return result;
}


/// Prepares test.
static uint32_t popcnt_u4_init(void)
{
    result = 0;
    popcnt_u4_test.heap = marsh_alloc(
        popcnt_u4_test.iterations * sizeof(uint64_t));
    uint64_t *buffer = (uint64_t*)popcnt_u4_test.heap;
    marsh_random_data(buffer, popcnt_u4_test.iterations);
}


/// Validates test's result.
static uint32_t popcnt_u4_verify(void)
{
    return result == popcnt_u4_golden ? 0 : 1;
}


/// Reports test's results.
static void popcnt_u4_report(double elapsed, uint32_t retries)
{
    if (result != popcnt_u4_golden)
        printf("Result: %8.8x Expected: %8.8x\n", result, popcnt_u4_golden);
    uint32_t n = popcnt_u4_test.iterations;
    printf("iterations=%d, data=%.3fKB, bandwidth: %.3f GB/s\n",
        n, n * sizeof(uint64_t) / 1e3,
        n * sizeof(uint64_t) * retries / elapsed / 1e9);
}

MARSH_TEST(popcnt_u4, popcnt_u4_run, popcnt_u4_init,
    popcnt_u4_verify, popcnt_u4_report);
