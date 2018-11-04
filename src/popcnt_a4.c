#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

const char* test_name = "popcnt";

#define TEST_N  (MARSH_L2C_SIZE / sizeof(uint64_t))
static uint64_t buffer[TEST_N];

#define GOLDEN  0x000f7fd7
static int result;

uint64_t run()
{
    uint64_t tmp0, tmp1, tmp2, tmp3;
    uint64_t cnt0, cnt1, cnt2, cnt3;
    cnt0 = cnt1 = cnt2 = cnt3 = 0;
    const uint64_t* buf = buffer;
    uint64_t len = TEST_N;
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
    result = cnt0 + cnt1 + cnt2 + cnt3;
    return result;
}

int init()
{
    marsh_random_data(buffer, MARSH_L2C_SIZE);
}

int verify()
{
    return result == GOLDEN ? 0 : 1;
}

void report(double elapsed, uint64_t retries)
{
    if (result != GOLDEN)
        printf("Result: %8.8x Expected: %8.8x\n", result, GOLDEN);
    printf("bandwidth: %.3f GB/s\n",
        (MARSH_L2C_SIZE * retries / elapsed / 1e9));
}
