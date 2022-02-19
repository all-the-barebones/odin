#include <stddef.h>
#include <stdint.h>

#ifdef __clang__
#pragma GCC diagnostic ignored "-Wmacro-redefined"
#endif

#define USE_LOCKS 1
#define USE_SPIN_LOCKS 1
#define HAVE_MORECORE 1
#define MORECORE gib_more_core
#define MORECORE_CANNOT_TRIM
#define HAVE_MMAP 0
#define HAVE_MREMAP 0
#define MMAP_CLEARS 0
#define LACKS_ERRNO_H
#define LACKS_FCNTL_H
#define LACKS_SCHED_H
#define LACKS_STDLIB_H
#define LACKS_STRINGS_H
#define LACKS_STRING_H
#define LACKS_SYS_MMAN_H
#define LACKS_SYS_PARAM_H
#define LACKS_SYS_TYPES_H
#define LACKS_TIME_H
#define LACKS_UNISTD_H
#define NO_MALLOC_STATS 1

#define ENOMEM 1
#define EINVAL 2
static int errno;
static inline void outb(uint16_t port, uint8_t val) { asm volatile("outb %0, %1" : : "a"(val), "Nd"(port)); }
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void debug(const char* s) {
    while (*s) outb(0xe9, *s++);
    outb(0xe9, '\n');
}
void debugc(char c) { outb(0xe9, c); }
__attribute__((noreturn)) void abort(void) {
    debug("abort()");
    while (1) asm("cli;hlt");
}

static size_t bump_cur;
static uint8_t mem[0x4000];
#define kMemSize 0x4000
static void* gib_more_core(size_t size) {
    bump_cur += size;
    if (bump_cur > kMemSize)
        while (1) asm("cli; hlt");
    return &mem[bump_cur - size];
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
    unsigned char* dst = (unsigned char*)dstptr;
    const unsigned char* src = (const unsigned char*)srcptr;
    for (size_t i = 0; i < size; i++) dst[i] = src[i];
    return dstptr;
}
void* memset(void* bufptr, int value, size_t size) {
    unsigned char* buf = (unsigned char*)bufptr;
    for (size_t i = 0; i < size; i++) buf[i] = (unsigned char)value;
    return bufptr;
}
int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*)aptr;
    const unsigned char* b = (const unsigned char*)bptr;
    for (size_t i = 0; i < size; i++) {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}
void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*)dstptr;
    const unsigned char* src = (const unsigned char*)srcptr;
    if (dst < src) {
        for (size_t i = 0; i < size; i++) dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--) dst[i - 1] = src[i - 1];
    }
    return dstptr;
}
#include <dlmalloc.h>
