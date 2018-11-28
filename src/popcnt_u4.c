#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test popcnt_u4_test;

const static uint32_t popcnt_u4_golden = 122941;

static uint32_t result;

/// Runs test.
static uint32_t popcnt_u4_run(void)
{
    uint64_t *buffer = (uint64_t*)popcnt_u4_test.x;
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
    marsh_get_x(&popcnt_u4_test, sizeof(uint64_t));
}


/// Validates test's result.
static uint32_t popcnt_u4_verify(void)
{
    return result == popcnt_u4_golden ? 0 : 1;
}


/// Reports test's results.
static void popcnt_u4_report(double elapsed, uint32_t retries)
{
    uint32_t errors = result == popcnt_u4_golden ? 0 : 1;
    double mark = popcnt_u4_test.read_size * retries / elapsed / 1e9;
    marsh_report(&popcnt_u4_test, errors, elapsed, retries, mark);
}

MARSH_TEST(popcnt_u4, popcnt_u4_run, popcnt_u4_init,
    popcnt_u4_verify, popcnt_u4_report);
