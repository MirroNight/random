#include <inttypes.h>
#include "../src/lfsr.h"
#include "../src/print_raw_uint.h"

int main(void){
    Lfsr32State lfsr_state;
    slfsr32_r(&lfsr_state, 5498);
    for(;;){
        uint32_t num = lfsr32_r(&lfsr_state);
        print_raw_uint32(num);
    }
    return 0;
}
