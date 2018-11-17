#include "marsh.h"

/// Statically allocated heap (to avoid malloc() where possible).
static uint8_t __marsh_heap[MARSH_HEAP_SIZE] = {0};
static uint8_t* __marsh_heap_p = __marsh_heap;


/// Helper for memory allocation.
void* marsh_alloc(uint32_t size)
{
    uint8_t *p;

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
        *pdw++ = rand();
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
