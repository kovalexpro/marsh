
#pragma once

#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

#ifndef STREAM_SET_FILLER
#define STREAM_SET_FILLER 0x00
#endif

extern struct marsh_test thistest;

static uint32_t result;

/// Runs test.
static uint32_t thisfunc(run)(void)
{
    datatype *buffer = (datatype*)thistest.heap;
    uint32_t n = thistest.iterations;
    for(int i = 0; i < n; i++) {
        buffer[i] = STREAM_SET_FILLER;
    }
    return (uint32_t)buffer[0];
}


/// Prepares test.
static uint32_t thisfunc(init)(void)
{
    result = 0;
    marsh_get_r(&thistest, sizeof(datatype));
}


/// Validates test's result.
static uint32_t thisfunc(verify)(void)
{
    result = 0;
    uint32_t n = thistest.iterations;
    datatype *buffer = (datatype*)thistest.heap;
    volatile datatype gold = STREAM_SET_FILLER;
    for(int i = 0; i < n; i++)
        result += (buffer[i] == gold) ? 0 : 1;
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
