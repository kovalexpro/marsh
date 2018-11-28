#include <stdio.h>
#include <stdint.h>
#include "marsh.h"


int main(int argc, char** argv)
{
    marsh_time_t start, stop;
    double warmup, elapsed;
    uint32_t accumulated = 0;

    // initializes test (and its data)
    test->init();

    // warmup
    start = marsh_time();
    for (int i = 0; i < MARSH_WARMUP_RETRY; i++) {
        accumulated += test->run();
    }
    stop = marsh_time();
    warmup = marsh_elapsed(start, stop);

    // timing run
    start = marsh_time();
    for (int i = 0; i < MARSH_RETRY; i++) {
        accumulated += test->run();
    }
    stop = marsh_time();
    elapsed = marsh_elapsed(start, stop);

    // validates results
    uint32_t err = test->verify();
    if (err) {
        printf("%s: test failed with %d error(s)\n", test->name, err);
    } else {
        printf("%s: test passed\n", test->name);
    }

    // test config
    printf("%s: marsh_config=%s %s %s %s\n", test->name,
        BUILD_VARIANT, BUILD_REV, BUILD_BRANCH, BUILD_DATE);

    // test stat
    printf("%s: accum=%u retry=%d/%d warmup=%.3fs elapsed=%.3fs\n",
        test->name, accumulated, MARSH_WARMUP_RETRY,
        MARSH_RETRY, warmup, elapsed);
    test->report(elapsed, MARSH_RETRY);

    return err;
}
