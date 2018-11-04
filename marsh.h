
#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

enum {
    MARSH_WARMUP_RETRY = 1000,
    MARSH_RETRY = 1000000,
    MARSH_L2C_SIZE = 256 * 1024,
};

static inline void marsh_random_data(void* p, uint32_t size)
{
    uint32_t* pdw = (uint32_t*)p;

    srand(size);
    for (int i = 0; i < size / sizeof(uint32_t); i++) {
        *pdw++ = rand();
    }

    uint8_t* pb = (uint8_t*)pdw;
    uint32_t rdw = rand();
    for (int i = 0; i < (size & (sizeof(uint32_t) - 1)); i++) {
        *pb++ = rdw & 0xff;
        rdw = rdw >> 8;
    }

}

typedef clock_t marsh_time_t;


static inline marsh_time_t marsh_time()
{
    return (marsh_time_t)clock();
}


static inline double marsh_elapsed(marsh_time_t start, marsh_time_t stop)
{
    return (double)(stop - start) / CLOCKS_PER_SEC;
}


extern const char* test_name;
extern uint64_t run();
extern int init();
extern int verify();
extern void report(double elapsed, uint64_t retries);
