
#include <stdint.h>

struct marsh_test {
    uint32_t iterations;
    const char* name;
    void* heap;
    void* private;
    uint32_t (*run)();
    uint32_t (*init)();
    uint32_t (*verify)();
    void (*report)(double elapsed, uint32_t retries);
};

#define MARSH_TEST(name, n, run, init, verify, report) \
    struct marsh_test name = { \
        .iterations = n, \
        .name = name##_name, \
        .heap = name##_heap, \
        .private = name##_params, \
        .run = run, \
        .init = init, \
        .verify = verify, \
        .report = report \
    };
