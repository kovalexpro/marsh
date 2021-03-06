
#pragma once

#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

extern struct marsh_test thistest;

static uint32_t result;

/// Runs test.
static uint32_t thisfunc(run)(void)
{
    uint32_t n = thistest.iterations;
    datatype *r = (datatype*)thistest.r;
    datatype *x = (datatype*)thistest.x;
    datatype *y = (datatype*)thistest.y;
    for(int i = 0; i < n; i++) {
        r[i] = x[i] + y[i];
    }
    return (uint32_t)r[n-1];
}


/// Prepares test.
static uint32_t thisfunc(init)(void)
{
    result = 0;
    marsh_get_r(&thistest, sizeof(datatype));
    marsh_get_xy(&thistest, sizeof(datatype));
}


/// Validates test's result.
static uint32_t thisfunc(verify)(void)
{
    result = 0;
    uint32_t n = thistest.iterations;
    datatype *r = (datatype*)thistest.r;
    datatype *x = (datatype*)thistest.x;
    datatype *y = (datatype*)thistest.y;
    for(int i = 0; i < thistest.iterations; i++) {
        volatile datatype gold = x[i] + y[i];
        result += (r[i] == gold) ? 0 : 1;
    }
    return result;
}


/// Reports test's results.
static void thisfunc(report)(double elapsed, uint32_t retries)
{
    double mark = test->write_size * retries / elapsed / 1e9;
    marsh_report(&thistest, result, elapsed, retries, mark);
}

THIS_MARSH_TEST(thisname, thisfunc(run),
    thisfunc(init), thisfunc(verify),
    thisfunc(report));
