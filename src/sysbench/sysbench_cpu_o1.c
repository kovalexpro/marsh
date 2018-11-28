#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "marsh.h"

extern struct marsh_test sysbench_cpu_o1_test;

const static uint32_t sysbench_cpu_golden = 167;

static uint32_t result;


/// Runs test.
static uint32_t sysbench_cpu_o1_run(void)
{
    double cd;
    uint64_t t;
    uint64_t c, l, i;
    uint64_t n = 0;

    // So far we're using very simple test prime number tests in 64bit
    for (c=3; c < sysbench_cpu_o1_test.iterations; c+=2)
    {
        t = (uint64_t)sqrt((double)c);

        i = 1;
        for (l = 3; l <= t; l+=2)
            if (c % l == 0) {
                i = 0;
                break;
            }

        n += i;
    }

    result = (uint32_t)(n & 0xfffffff);
    return result;
}


/// Prepares test.
static uint32_t sysbench_cpu_o1_init(void)
{
    result = 0;
    sysbench_cpu_o1_test.iterations = 1000;
}


/// Validates test's result.
static uint32_t sysbench_cpu_o1_verify(void)
{
    return result == sysbench_cpu_golden ? 0 : 1;
}


/// Reports test's results.
static void sysbench_cpu_o1_report(double elapsed, uint32_t retries)
{
    uint32_t errors = result == sysbench_cpu_golden ? 0 : 1;
    uint32_t n = sysbench_cpu_o1_test.iterations;
    printf("%s: errors=%u iterations=%d npi=%.3f mark=%.3f Mruns/s\n",
        sysbench_cpu_o1_test.name, errors, n, elapsed * 1e9 / retries / n,
        result * retries / elapsed / 1e6 );
}

MARSH_TEST(sysbench_cpu_o1, sysbench_cpu_o1_run, sysbench_cpu_o1_init,
    sysbench_cpu_o1_verify, sysbench_cpu_o1_report);
