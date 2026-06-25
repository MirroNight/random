#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include "../src/bytes2lf.h"

int main() {
    FILE *fh = stdin;
    U64_Lf x;
    for (;;) {
        // x.lf = bytes2lf_division(fh);
        // x.lf = bytes2lf_multiply(fh);
        // x.lf = bytes2lf_fill_mantissa(fh);
        x.lf = bytes2lf_exponential(fh);
        if (isnan(x.lf)) {
            break;
        }
        printf("%lf\t" "%16lx\n", x.lf, x.ul);
    }
    return 0;
}
