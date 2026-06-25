#include <inttypes.h>
#include "../src/mt19937.h"
#include "../src/print_raw_uint.h"

int main() {
    // mt19937 (Mersenne Twister)
    MtState state = {.idx = 0};
    smt19937_r(&state, 5489ul);
    for (;;) {
        uint32_t num = mt19937_r(&state);
        print_raw_uint32(num);
    }
    return 0;
}
