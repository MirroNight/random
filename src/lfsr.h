#include <stdint.h>

/* tap bits lookup table that achive max period
 * u8    8,  6,  5,  4  0xb8
 * u16  16, 14, 13, 11  0xb400
 * u32  32, 22,  2,  1  0x8020'0003
 * u64  64, 63, 61, 60  0xd800'0000'0000'0000
 */

#ifndef LFSR
#define LFSR

#define LFSR32_TAP 0x80200003

typedef struct lfsr32state Lfsr32State;

struct lfsr32state {
    uint32_t s;
};

void slfsr32_r(Lfsr32State *state, uint32_t seed) {
    static uint32_t last_seed = 0;
    if (!last_seed && seed == last_seed) {
        seed++;
    }
    state->s = seed ? seed : 5489;
    last_seed = seed ? seed : 5489;
}

uint32_t lfsr32_r(Lfsr32State *state) {
    uint32_t lsb = state->s & 1;
    state->s >>= 1;
    if (lsb) {
        state->s ^= LFSR32_TAP;
    }
    return state->s;
}

#endif

/*
#include <stdio.h>
int main(void) {
    Lfsr32State state;
    slfsr32_r(&state, 5498);
    for(;;) {
        uint32_t num = lfsr32_r(&state);
        putc((num & 0xff00'0000) >> 24, stdout);
        putc((num & 0x00ff'0000) >> 16, stdout);
        putc((num & 0x0000'ff00) >>  8, stdout);
        putc((num & 0x0000'00ff) >>  0, stdout);
    }
    return 0;
}
*/
