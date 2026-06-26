#include <inttypes.h>
#include "../src/pcg_variants.h"
#include "../src/print_raw_uint.h"

int main() {
    // pcg_random
    // u in pcg32u means unique, which all generates different sequence even if the seed is the same
    // to ensure reproducability, please use the variant without u
    pcg32u_random_t pcg32u_state;
    // pcg32_random_t pcg32_state;
    pcg32u_srandom_r(&pcg32u_state, 5489ul);
    // pcg32_srandom_r(&pcg32_state, 5489ul, 0);
    for (;;) {
        uint32_t num = pcg32u_random_r(&pcg32u_state);
        // uint32_t num = pcg32_random_r(&pcg32_state);
        print_raw_uint32(num);
    }
    return 0;
}
