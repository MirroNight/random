#include <inttypes.h>
#include "../src/xorshift.h"
#include "../src/print_raw_uint.h"

int main() {
    // xorshift
    XorShift32State xorshift32_state;
    sxorshift32_r(&xorshift32_state, 5489ul);
    for (;;) {
        uint32_t num = xorshift32_r(&xorshift32_state);
        print_raw_uint32(num);
    }
    return 0;
}
