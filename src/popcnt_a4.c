#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test popcnt_a4_test;

const static uint32_t popcnt_a4_golden = 0x1f018;

static uint32_t result;

/// Runs test.
static uint32_t popcnt_a4_run(void)
{
    uint64_t *buffer = (uint64_t*)popcnt_a4_test.heap;
    uint64_t tmp0, tmp1, tmp2, tmp3;
    uint64_t cnt0, cnt1, cnt2, cnt3;
    cnt0 = cnt1 = cnt2 = cnt3 = 0;
    const uint64_t* buf = buffer;
    uint64_t len = popcnt_a4_test.iterations;
    __asm__ __volatile__ (
        "1:\n\t"
        "popcnt (%%rdi), %1\n\t"
        "popcnt 8(%%rdi), %2\n\t"
        "popcnt 16(%%rdi), %3\n\t"
        "popcnt 24(%%rdi), %4\n\t"
        "leaq 32(%%rdi), %%rdi\n\t"
        "addq %1, %5\n\t"
        "addq %2, %6\n\t"
        "addq %3, %7\n\t"
        "addq %4, %8\n\t"
        "subq $4, %0\n\t"
        "jnz 1b\n\t"
        : "+q" (len), "=q" (tmp0), "=q" (tmp1), "=q" (tmp2), "=q" (tmp3),
          "+q" (cnt0), "+q" (cnt1), "+q" (cnt2), "+q" (cnt3),
          "+D" (buf)
        :
        :
    );
    uint64_t cnt = cnt0 + cnt1 + cnt2 + cnt3;
    result = (uint32_t)(cnt & 0xfffffff) ^ (uint32_t)(cnt >> 32);
    return result;
}


/// Prepares test.
static uint32_t popcnt_a4_init(void)
{
    result = 0;
    popcnt_a4_test.heap = marsh_alloc(
        popcnt_a4_test.iterations * sizeof(uint64_t));
    uint64_t *buffer = (uint64_t*)popcnt_a4_test.heap;
    marsh_random_data(buffer, popcnt_a4_test.iterations);
}


/// Validates test's result.
static uint32_t popcnt_a4_verify(void)
{
    return result == popcnt_a4_golden ? 0 : 1;
}


/// Reports test's results.
static void popcnt_a4_report(double elapsed, uint32_t retries)
{
    if (result != popcnt_a4_golden)
        printf("Result: %8.8x Expected: %8.8x\n", result, popcnt_a4_golden);
    uint32_t n = popcnt_a4_test.iterations;
    printf("iterations=%d, data=%.3fKB, bandwidth: %.3f GB/s\n",
        n, n * sizeof(uint64_t) / 1e3,
        n * sizeof(uint64_t) * retries / elapsed / 1e9);
}

MARSH_TEST(popcnt_a4, popcnt_a4_run, popcnt_a4_init,
    popcnt_a4_verify, popcnt_a4_report);
