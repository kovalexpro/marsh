
#pragma once

#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

#ifndef STREAM_SET_SCALE
#define STREAM_SET_SCALE 1
#endif


extern struct marsh_test thistest;

static uint32_t result;

/// Runs test.
static uint32_t thisfunc(run)(void)
{
    uint32_t n = thistest.iterations;
    datatype *to_buf = (datatype*)thistest.r;
    datatype *from_buf = (datatype*)thistest.x;
    for(int i = 0; i < n; i++) {
        to_buf[i] = from_buf[i] * STREAM_SET_SCALE;
    }
    return (uint32_t)to_buf[0];
}


/// Prepares test.
static uint32_t thisfunc(init)(void)
{
    result = 0;
    marsh_get_r(&thistest, sizeof(datatype));
    marsh_get_x(&thistest, sizeof(datatype));
}


/// Validates test's result.
static uint32_t thisfunc(verify)(void)
{
    result = 0;
    uint32_t n = thistest.iterations;
    datatype *to_buf = (datatype*)thistest.r;
    datatype *from_buf = (datatype*)thistest.x;
    for(int i = 0; i < thistest.iterations; i++)
        result += (to_buf[i] == from_buf[i] * STREAM_SET_SCALE) ? 0 : 1;
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
