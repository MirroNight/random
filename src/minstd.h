#include <stdint.h>

#ifndef MINSTD
#define MINSTD

typedef struct lcg32state Lcg32State;

void slcg_r(Lcg32State *state, uint32_t seed);
uint32_t minstd_r(Lcg32State *state);

struct lcg32state {
    uint32_t s;
};

void slcg_r(Lcg32State *state, uint32_t seed) {
    // defualt seed 5489 is used when 0 is provided
    // using same seed in consecutive call will increment new seed by 1
    static uint32_t last_seed = 0;
    if (!last_seed && seed == last_seed) {
        seed++;
    }
    state->s = seed ? seed : 5489;
    last_seed = seed ? seed : 5489;
}

uint32_t minstd_r(Lcg32State *state) {
    return state->s = (uint32_t) ((uint64_t)(state->s) * 16807 + 0) % (2147483647);
}

#endif

/*
#include <stdio.h>
int main(void) {
    Lcg32State state;
    slcg_r(&state, 5498);
    for(;;) {
        uint32_t num = minstd_r(&state);
        putc((num & 0xff00'0000) >> 24, stdout);
        putc((num & 0x00ff'0000) >> 16, stdout);
        putc((num & 0x0000'ff00) >>  8, stdout);
        putc((num & 0x0000'00ff) >>  0, stdout);
    }
    return 0;
}
*/
