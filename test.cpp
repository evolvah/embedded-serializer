#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "ByteArray.h"

void test_ByteArray() {
    char        buff[30];
    ByteArray ba(buff, 30);

    // - - - - - - - - - - - - - - - - - - - - - -
    uint32_t    d1 = 1;
    const char* s1 = "blah-blah!";
    uint16_t    d2 = 2;
    uint8_t     d3 = 3;
    double      d4 = 0.123467;

    printf("Serializing  : x1: %d, r1: '%s' x2: %d, x3: %d, x4: %lf\n", d1, s1, d2, d3, d4);
    ba << d1 << s1 << d2 << d3 << d4;
    printf("ok: %s, capacity: %d , position: %d \n", (ba.IsOk() ? "true" : "false"), ba.GetCapacity(), ba.GetPosition());
    assert(ba.IsOk());

    // - - - - - - - - - - - - - - - - - - - - - -
    uint32_t x1 = 0;
    char     r1[20];
    uint16_t x2 = 0;
    uint8_t  x3 = 0;
    double   x4 = 0.0;

    ba.Reset();
    ba >> x1 >> r1 >> x2 >> x3 >> x4;

    printf("Deserializing: x1: %d, r1: '%s' x2: %d, x3: %d, x4: %lf\n", x1, r1, x2, x3, x4);
    assert(x1 == d1);
    assert(x2 == d2);
    assert(x3 == d3);
    assert(strcmp(s1, r1) == 0);

    // - - - - - - - - - - - - - - - - - - - - - -
    printf("Testing overflow\n");
    ba << s1 << s1 << s1 << s1 << s1;
    printf("ok: %s\n", (ba.IsOk() ? "true" : "false"));
    assert(ba.IsOk() == false);
}

int main(int argc, char const *argv[])
{
    test_ByteArray();

    return 0;
}