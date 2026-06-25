#include <stdio.h>
#include <inttypes.h>

#ifndef PRINT_RAW_INT
#define PRINT_RAW_INT

void print_raw_uint32(uint32_t n) {
    putc((n & 0xff00'0000) >> 24, stdout);
    putc((n & 0x00ff'0000) >> 16, stdout);
    putc((n & 0x0000'ff00) >>  8, stdout);
    putc((n & 0x0000'00ff) >>  0, stdout);
}
#endif

