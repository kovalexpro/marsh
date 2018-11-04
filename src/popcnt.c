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
    uint64_t cnt = 0;
    for(int i = 0; i < TEST_N; i++) {
        cnt += __builtin_popcountll(buffer[i]);
    }
    result = cnt;
    return cnt;
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
