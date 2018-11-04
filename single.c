#include <stdio.h>
#include <stdint.h>
#include "marsh.h"

int main(int argc, char** argv)
{
    marsh_time_t start, stop;
    double warmup, elapsed;
    uint64_t count = 0;

    // initializes test (and its data)
    init();

    // warmup
    start = marsh_time();
    for (int i = 0; i < MARSH_WARMUP_RETRY; i++) {
        count += run();
    }
    stop = marsh_time();
    warmup = marsh_elapsed(start, stop);

    // timing run
    start = marsh_time();
    for (int i = 0; i < MARSH_RETRY; i++) {
        count += run();
    }
    stop = marsh_time();
    elapsed = marsh_elapsed(start, stop);

    // validates results
    int err = verify();
    if (err) {
        printf("%s: test failed with %d error(s)\n", test_name, err);
    } else {
        printf("%s: test passed\n", test_name);
    }

    // test stat
    printf("%s: count=%ld retry=%d/%d warmup=%.3fs elapsed=%.3fs\n",
        test_name, count, MARSH_WARMUP_RETRY, MARSH_RETRY, warmup, elapsed);
    report(elapsed, MARSH_RETRY);
}
