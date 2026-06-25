#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef CHACHA20
#define CHACHA20

#define CHACHA20_BLOCK_SIZE 64

// rotate bits left
#define ROTL32(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

// quarter round operation
#define QUARTER_ROUND(a, b, c, d) \
    a += b; d ^= a; d = ROTL32(d, 16); \
    c += d; b ^= c; b = ROTL32(b, 12); \
    a += b; d ^= a; d = ROTL32(d, 8);  \
    c += d; b ^= c; b = ROTL32(b, 7);

typedef struct chacha20 ChaCha20Ctx;

struct chacha20 {
    uint32_t state[16];
    uint8_t  buf[CHACHA20_BLOCK_SIZE];
    size_t   idx;
};

void schacha20_r(ChaCha20Ctx *ctx, const uint8_t seed[32], const uint8_t nonce[12]) {
    // RFC 8439 4-word constant "expand 32-byte k"
    ctx->state[0] = 0x61707865; // "expa"
    ctx->state[1] = 0x3320646e; // "nd 3"
    ctx->state[2] = 0x79622d32; // "2-by"
    ctx->state[3] = 0x6b206574; // "te k"

    // set the 256-bits key from the seed (8 * 32)
    for (int i = 0; i < 8; i++) {
        ctx->state[4 + i] = ((uint32_t) seed[i * 4 + 0])       |
                            ((uint32_t) seed[i * 4 + 1] << 8)  |
                            ((uint32_t) seed[i * 4 + 2] << 16) |
                            ((uint32_t) seed[i * 4 + 3] << 24);
    }

    // set count to 0
    ctx->state[12] = 0;

    // set nonce when offered
    if (nonce != NULL) {
        for (int i = 0; i < 3; i++) {
            ctx->state[13 + i] = ((uint32_t)nonce[i * 4 + 0])       |
                                 ((uint32_t)nonce[i * 4 + 1] << 8)  |
                                 ((uint32_t)nonce[i * 4 + 2] << 16) |
                                 ((uint32_t)nonce[i * 4 + 3] << 24);
        }
    } else {
        ctx->state[13] = 0;
        ctx->state[14] = 0;
        ctx->state[15] = 0;
    }

    // init buffer
    ctx->idx = CHACHA20_BLOCK_SIZE; 
    memset(ctx->buf, 0, CHACHA20_BLOCK_SIZE);
}

static void chacha20_generate_block(ChaCha20Ctx *ctx) {
    uint32_t mix[16];
    memcpy(mix, ctx->state, sizeof(mix));

    // 20 rounds (10 column - diagonal pairs)
    for (int i = 0; i < 10; i++) {
        // column
        QUARTER_ROUND(mix[0], mix[4], mix[8],  mix[12]);
        QUARTER_ROUND(mix[1], mix[5], mix[9],  mix[13]);
        QUARTER_ROUND(mix[2], mix[6], mix[10], mix[14]);
        QUARTER_ROUND(mix[3], mix[7], mix[11], mix[15]);
        // diagonal
        QUARTER_ROUND(mix[0], mix[5], mix[10], mix[15]);
        QUARTER_ROUND(mix[1], mix[6], mix[11], mix[12]);
        QUARTER_ROUND(mix[2], mix[7], mix[8],  mix[13]);
        QUARTER_ROUND(mix[3], mix[4], mix[9],  mix[14]);
    }

    // add state to mix
    for (int i = 0; i < 16; i++) {
        mix[i] += ctx->state[i];
    }

    // load mixed number into uint8_t[64] buffer (consider little endian)
    for (int i = 0; i < 16; i++) {
        ctx->buf[i * 4 + 0] = (uint8_t)(mix[i] & 0xFF);
        ctx->buf[i * 4 + 1] = (uint8_t)((mix[i] >> 8) & 0xFF);
        ctx->buf[i * 4 + 2] = (uint8_t)((mix[i] >> 16) & 0xFF);
        ctx->buf[i * 4 + 3] = (uint8_t)((mix[i] >> 24) & 0xFF);
    }

    // increment counter, reset buffer index
    ctx->state[12]++;
    ctx->idx = 0;
}

// get arbitrary bytes
void chacha20_get_n_bytes_r(ChaCha20Ctx *ctx, uint8_t *out_bytes, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (ctx->idx >= CHACHA20_BLOCK_SIZE) {
            chacha20_generate_block(ctx);
        }
        out_bytes[i] = ctx->buf[ctx->idx++];
    }
}

// get u32
uint32_t chacha20_get_u32_r(ChaCha20Ctx *ctx) {
    uint32_t result;
    chacha20_get_n_bytes_r(ctx, (uint8_t*) &result, sizeof(result));
    return result;
}

#endif

/*
int main(void) {
    ChaCha20Ctx prng;

    uint8_t my_seed[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };

    schacha20_r(&prng, my_seed, NULL);

    for (;;) {
        uint32_t num = chacha20_get_u32_r(&prng);
        putc((num & 0xff00'0000) >> 24, stdout);
        putc((num & 0x00ff'0000) >> 16, stdout);
        putc((num & 0x0000'ff00) >>  8, stdout);
        putc((num & 0x0000'00ff) >>  0, stdout);
    }

    return 0;
}
*/
