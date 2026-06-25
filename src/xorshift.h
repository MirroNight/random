#include <stdint.h>

#ifndef XORSHIFT
#define XORSHIFT

typedef struct xorshift32_s XorShift32State;
typedef struct xorshift64_s XorShift64State;

void sxorshift32_r(XorShift32State *state, uint32_t seed);
void sxorshift64_r(XorShift64State *state, uint64_t seed);
uint32_t xorshift32_r(XorShift32State *state);
uint64_t xorshift64_r(XorShift64State *state);

struct xorshift32_s {
    uint32_t s;
};

struct xorshift64_s {
    uint64_t s;
};

void sxorshift32_r(XorShift32State *state, uint32_t seed) {
    // default seed 5489 is used when 0 is provided.
    // using same seed in consecutive call will increment new seed by 1
    static uint32_t last_seed = 0;
    if (!last_seed && seed == last_seed) {
        seed++;
    }
    state->s = seed ? seed : 5489;
    last_seed = seed ? seed : 5489;
}

void sxorshift64_r(XorShift64State *state, uint64_t seed) {
    // default seed 5489 is used when 0 is provided.
    // using same seed in consecutive call will increment new seed by 1
    static uint64_t last_seed = 0;
    if (!last_seed && seed == last_seed) {
        seed++;
    }
    state->s = seed ? seed : 5489;
    last_seed = seed ? seed : 5489;
}

uint32_t xorshift32_r(XorShift32State *state) {
    // period 2^32-1
    uint32_t x = state->s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return state->s = x;
}

uint64_t xorshift64_r(XorShift64State *state) {
    // period 2^64-1
    uint64_t x = state->s;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return state->s = x;
}

#endif

/*
#include <stdio.h>
int main(void) {
    XorShift32State state;
    sxorshift32_r(&state, 5489);
    for(;;) {
        uint32_t num = xorshift32_r(&state);
        putc((num & 0xff00'0000) >> 24, stdout);
        putc((num & 0x00ff'0000) >> 16, stdout);
        putc((num & 0x0000'ff00) >>  8, stdout);
        putc((num & 0x0000'00ff) >>  0, stdout);
    }
    return 0;
}
*/

