#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

enum {
    DATA_SIZE = 8
};

extern struct marsh_test pchase_test;

typedef struct block {
    struct block *next;
    uint8_t data[DATA_SIZE];
} block_t;

static block_t* result;


/// Runs test.
static uint32_t pchase_run(void)
{
    block_t *p = (block_t*)pchase_test.heap;
    while (p->next)
        p = p->next;
    result = p;
    return 1;
}


/// Prepares test.
static uint32_t pchase_init(void)
{
    result = 0;

    uint64_t n = pchase_test.iterations;
    pchase_test.heap = marsh_alloc(n * sizeof(block_t));
    pchase_test.read_size = n * sizeof(struct block *);

    block_t *blocks = (block_t*)pchase_test.heap;
    for (int i = 0; i < (n - 1); i++) {
        blocks[i].next = &blocks[i + 1];
        #ifndef PCHASE_TEST_SKIP_INIT_DATA
        for (int j = 0; j < DATA_SIZE; j++)
            blocks[i].data[j] = (uint8_t)((i >> (8 * j)) & 0xff);
        #endif
    }
    blocks[n - 1].next = 0;

    return 0;
}


/// Validates test's result.
static uint32_t pchase_verify(void)
{
    uint64_t n = pchase_test.iterations;
    block_t *blocks = (block_t*)pchase_test.heap;
    return result == &blocks[n - 1] ? 0 : 1;
}


/// Reports test's results.
static void pchase_report(double elapsed, uint32_t retries)
{
    uint64_t n = pchase_test.iterations;
    block_t *blocks = (block_t*)pchase_test.heap;
    uint32_t errors = (result == &blocks[n - 1]) ? 0 : 1;
    double mark = test->read_size * retries / elapsed / 1e9;
    marsh_report(&pchase_test, errors, elapsed, retries, mark);
}

MARSH_TEST(pchase, pchase_run, pchase_init, pchase_verify, pchase_report);
