#include <inttypes.h>
#include <math.h>
#include <stdio.h>

#ifndef BYTES2LF
#define BYTES2LF

typedef union u64_lf U64_Lf;

union u64_lf {
    double lf;
    uint64_t ul;
};

double bytes2lf_division(FILE *fh) {
    int i = 0;
    int c = 0;
    uint64_t rand_bits = 0;

    while ((c = fgetc(fh)) != EOF) {
        if (i < 6) {
            rand_bits |= ((uint64_t) (c & 0xff)) << (i * 8);
            i++;
        } else {
            rand_bits |= ((uint64_t) (c & 0x0f)) << 48;
            return (double) rand_bits / 4503599627370496.0;
        }
    }
    return NAN;
}

double bytes2lf_multiply(FILE *fh) {
    int i = 0;
    int c = 0;
    uint64_t rand_bits = 0;

    while ((c = fgetc(fh)) != EOF) {
        if (i < 6) {
            rand_bits |= ((uint64_t) (c & 0xff)) << (i * 8);
            i++;
        } else {
            rand_bits |= ((uint64_t) (c & 0x0f)) << 48;
            return (double) rand_bits * 0x1p-52; 
        }
    }
    return NAN;
}

double bytes2lf_fill_mantissa(FILE* fh) {
    int i = 0;
    int c = 0;
    U64_Lf n = {.ul = 0x3ff0'0000'0000'0000};
    
    while ((c = fgetc(fh)) != EOF) {
        if (i < 6) {
            n.ul |= ((uint64_t) (c & 0xff)) << (i * 8);
            i++;
        } else {
            n.ul |= ((uint64_t) (c & 0x0f)) << 48;
            return n.lf - 1.0;
        }
    }
    return NAN;
}

double bytes2lf_exponential(FILE *fh) {
    int c;
    U64_Lf n;
    uint32_t leading_zeros = 0;

    // determine exponent with leading zeros count
    for (;;) {
        if ((c = fgetc(fh)) == EOF) {
            return NAN;
        }

        if ((c & 0xff) != 0) {
            // add to leading zeros count
            leading_zeros += (__builtin_clz(c & 0xff) - 24);
            break;
        } else {
            leading_zeros += 8;
            // exponent too small
            if (leading_zeros >= 1074) {
                return 0.0;
            }
        }
    }
    int64_t exponent = 1023 - 1 - leading_zeros; // exponent_max = 1023

    // return 0 when exponent is too small
    if (exponent <= 0) {
        return 0.0;
    }

    // fill mantissa
    uint64_t mantissa = 0;
    for (int i = 0; i < 6; i++) {
        if ((c = fgetc(fh)) == EOF) {
            return NAN;
        }
        mantissa |= ((uint64_t) (c & 0xff)) << (i * 8);

    }
    if ((c = fgetc(fh)) == EOF) {
        return NAN;
    }
    mantissa |= ((uint64_t) (c & 0x0f)) << 48;

    n.ul = (mantissa & 0x000f'ffff'ffff'ffff) | (exponent << 52);

    return n.lf;
}

#endif

/*
int mian() {
    FILE *fh = stdin;
    U64_Lf x;
    for (;;) {
        // x.lf = bytes2lf_division(fh);
        // x.lf = bytes2lf_multiply(fh);
        // x.lf = bytes2lf_fill_mantissa(fh);
        x.lf = bytes2lf_exponential(fh);
        printf("%lf\n", x.lf);
    }
    return 0;
}
*/
