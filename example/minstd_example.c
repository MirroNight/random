#include <inttypes.h>
#include "../src/minstd.h"
#include "../src/print_raw_uint.h"

int main() {
    Lcg32State minstd_state;
    slcg_r(&minstd_state, 5498);
    for (;;) {
        uint32_t num = minstd_r(&minstd_state);
        print_raw_uint32(num);
    }
    return 0;
}
