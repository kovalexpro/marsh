#include "marsh.h"

/// Statically allocated heap (to avoid malloc() where possible).
static uint8_t __marsh_heap[MARSH_HEAP_SIZE] = {0};
static uint8_t* __marsh_heap_p = __marsh_heap;


/// Helper for memory allocation.
void* marsh_alloc(uint32_t size)
{
    uint8_t *p;

    // check alignment (should be 2**n)
    const uintptr_t a = (uintptr_t)MARSH_ALLOC_ALIGN;
    const uintptr_t m = ~(a - 1);
    if ((uintptr_t)__marsh_heap_p & m)
        __marsh_heap_p = (uint8_t*)(((uintptr_t)__marsh_heap_p & m) + a);

    // first, try static allocation
    uint32_t used = __marsh_heap_p - __marsh_heap;
    uint32_t left = MARSH_HEAP_SIZE - used;
    if (size < left) {
        p = __marsh_heap_p;
        __marsh_heap_p += size;
        return (void*)__marsh_heap_p;
    }

    // slow path
    return malloc(size);
}


/// Helper for memory de-allocation.
void marsh_free(void* p)
{
    uint8_t* pc = (uint8_t*)p;
    if ((pc < __marsh_heap) || (pc > __marsh_heap + MARSH_HEAP_SIZE))
        free(p);
}


/// Fill given buffer with random data.
void marsh_random_data(void* p, uint32_t size)
{
    uint32_t* pdw = (uint32_t*)p;

    srand(size);
    for (int i = 0; i < size / sizeof(uint32_t); i++) {
        *pdw++ = rand() & 0x7f7fffff;
    }

    uint8_t* pb = (uint8_t*)pdw;
    uint32_t rdw = rand();
    for (int i = 0; i < (size & (sizeof(uint32_t) - 1)); i++) {
        *pb++ = rdw & 0xff;
        rdw = rdw >> 8;
    }

}


/// Timinig routine.
marsh_time_t marsh_time()
{
    return (marsh_time_t)clock();
}


/// Returns elapsed time in seconds using current timing routine.
double marsh_elapsed(marsh_time_t start, marsh_time_t stop)
{
    return (double)(stop - start) / CLOCKS_PER_SEC;
}


/// Standard report routine.
void marsh_report(struct marsh_test *test, uint32_t errors,
    double elapsed, uint32_t retries, double mark)
{
    uint32_t n = test->iterations;
    printf("%s: errors=%u iterations=%d npe=%.3f mark=%.3f\n",
        test->name, errors, n, elapsed * 1e9 / retries / n, mark);
    printf("%s: data[r/w]=%.3f/%.3fKB bandwidth[r/w]=%.3f/%.3fGB/s\n",
        test->name,
        test->read_size / 1e3,
        test->write_size / 1e3,
        test->read_size * retries / elapsed / 1e9,
        test->write_size * retries / elapsed / 1e9
    );
}


/// Allocates and initializes resulting vector.
void marsh_get_r(struct marsh_test *test, uint32_t elem_size)
{
    test->r = marsh_alloc(test->iterations * elem_size);
    marsh_random_data(test->r, test->iterations);
    test->write_size += test->iterations * elem_size;
}


/// Allocates and initializes vector.
void marsh_get_x(struct marsh_test *test, uint32_t elem_size)
{
    test->x = marsh_alloc(test->iterations * elem_size);
    marsh_random_data(test->x, test->iterations);
    test->read_size += test->iterations * elem_size;
}


/// Allocates and initializes 2 vectors
void marsh_get_xy(struct marsh_test *test, uint32_t elem_size)
{
    uint32_t vsize = test->iterations * elem_size;
    test->x = marsh_alloc(vsize);
    void* misalign = marsh_alloc(MARSH_ALLOC_STRIDE * elem_size);
    test->y = marsh_alloc(vsize);
    marsh_random_data(test->x, vsize);
    marsh_random_data(test->y, vsize);
    test->read_size += 2 * vsize;
}

/// Allocates and initializes 3 vectors
void marsh_get_xyz(struct marsh_test *test, uint32_t elem_size)
{
    uint32_t vsize = test->iterations * elem_size;
    test->x = marsh_alloc(vsize);
    void* misalign = marsh_alloc(MARSH_ALLOC_STRIDE * elem_size);
    test->y = marsh_alloc(vsize);
    void* misalign2 = marsh_alloc(MARSH_ALLOC_STRIDE * elem_size);
    test->z = marsh_alloc(vsize);
    marsh_random_data(test->x, vsize);
    marsh_random_data(test->y, vsize);
    marsh_random_data(test->z, vsize);
    test->read_size += 3 * vsize;
}
