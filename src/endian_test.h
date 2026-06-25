#include <stdio.h>
#include <inttypes.h>

union u16_chars {
    uint16_t u16;
    unsigned char c[2];
};

int test_endian() {
   union u16_chars n = {.u16 = 0x1234};
    if (
            n.c[0] == 0x34
        &&  n.c[1] == 0x12
       ) {
        printf("Little endian\n");
    } else {
        printf("Big endian\n");
    }
    return 0;
}

/*
int main() {
    test_endian();
    return 0;
}
*/
