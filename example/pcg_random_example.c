#include <inttypes.h>
#include "../src/pcg_variants.h"
#include "../src/print_raw_uint.h"

int main() {
    // pcg_random
    pcg32u_random_t pcg32u_state;
    pcg32u_srandom_r(&pcg32u_state, 5489ul);
    for (;;) {
        uint32_t num = pcg32u_random_r(&pcg32u_state);
        print_raw_uint32(num);
    }
    return 0;
}
