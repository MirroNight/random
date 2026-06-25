#include <stdint.h>

#ifndef MT19937
#define MT19937

typedef struct mt_state_s MtState;

#define MT_LEN 624
#define MT_OFFSET 397
#define MT_MATRIX_A 0x9908'b0df
#define MT_UPPER_MASK_32 0x8000'0000
#define MT_LOWER_MASK_32 0x7fff'ffff

struct mt_state_s {
    int32_t idx;
    uint32_t mt[MT_LEN];
};

void smt19937_r(MtState *mts, uint32_t seed) {
    // default seed 5489 is used when 0 is provided.
    // using same seed in consecutive call will increment new seed by 1
    static uint32_t last_seed = 0;
    if (!last_seed && seed == last_seed) {
        seed++;
    }
    mts->idx = 0;
    mts->mt[0] = (seed ? seed : 5489);
    for (int32_t i = 1; i < MT_LEN; i++) {
        mts->mt[i] = 1'812'433'253 * (mts->mt[i-1] ^ (mts->mt[i-1] >> 30)) + i;
    }
    last_seed = seed ? seed : 5489;
}

uint32_t mt19937_r(MtState *mts) {
    uint32_t y;
    static uint32_t mag01[2] = {0x0, MT_MATRIX_A}; // (y & 0x1) ? MT_MATRIX_A : 0x0

    if (mts->idx >= MT_LEN) {
        // twist
        int32_t i;

        /* 
         * y = mt[0 ... ] | mt[1 ... 227]
         * mt[0 ... 226] = mt[397 ... 623] ^ (y>>1) ^ A
         */
        for (i = 0; i < MT_LEN - MT_OFFSET; i++) {
            y = (mts->mt[i] & MT_UPPER_MASK_32) | (mts->mt[i+1] & MT_LOWER_MASK_32);
            mts->mt[i] = mts->mt[i + MT_OFFSET] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        /*
         * y = mt[227 ... 622] | mt[228 ... 623]
         * mt[227 ... 622] = mt[0 ... 395] ^ (y>>1) ^ A
         */
        for (; i < MT_LEN - 1; i++) {
            y = (mts->mt[i] & MT_UPPER_MASK_32) | (mts->mt[i+1] & MT_LOWER_MASK_32);
            mts->mt[i] = mts->mt[i + MT_OFFSET - MT_LEN] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        /*
         * y = mt[623] | mt[0]
         * mt[623] = mt[396] ^ (y>>1) ^ A
         */
        y = (mts->mt[MT_LEN - 1] & MT_UPPER_MASK_32) | (mts->mt[0] & MT_LOWER_MASK_32);
        mts->mt[MT_LEN - 1] = mts->mt[MT_OFFSET - 1] ^ (y >> 1) ^ mag01[y & 0x1];

        mts->idx = 0;
    }

    y = mts->mt[mts->idx++];

    // tempering
    y ^= (y >> 11);
    y ^= (y <<  7) & 0x9d2c'5680;
    y ^= (y << 15) & 0xefc6'0000;
    y ^= (y << 18);

    return y;
}

#endif

/*
#include <stdio.h>
int main(void) {
    MtState state = {.idx = 0};
    smt19937_r(&state, 5489);
    for(;;) {
        uint32_t num = mt19937_r(&state);
        putc((num & 0xff00'0000) >> 24, stdout);
        putc((num & 0x00ff'0000) >> 16, stdout);
        putc((num & 0x0000'ff00) >>  8, stdout);
        putc((num & 0x0000'00ff) >>  0, stdout);
    }
    return 0;
}
*/
