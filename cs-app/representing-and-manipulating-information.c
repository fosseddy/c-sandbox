#include <stdio.h>
#include <assert.h>

int is_little_endian()
{
    int a = 0xFF;
    unsigned char *ptr = (unsigned char *) &a;

    return ptr[0] == 0xFF;
}

// Write a procedure is_little_endian that will return 1 when compiled and run
// on a little-endian machine, and will return 0 when compiled and run on a
// big-endian machine. This program should run on any machine,
// regardless of its word size.
void ex_2_58()
{
    assert(is_little_endian() == 1);
}

// Write a C expression that will yield a word consisting of the least
// significant byte of x and the remaining bytes of y.
void ex_2_59()
{
    int x = 0x89ABCDEF;
    int y = 0x76543210;

    assert((y ^ (y & 0xFF)) | (x & 0xFF) == 0x765432EF);
}

unsigned replace_byte(unsigned x, int i, unsigned char b)
{
    unsigned char *ptr = (unsigned char *) &x;
    ptr[i] = b;

    return x;
}

// Suppose we number the bytes in a w-bit word
// from 0 (least significant) to w/8 − 1 (most significant).
// Write code for the following C function, which will return an
// unsigned value in which byte i of argument x has been replaced by byte b:
// unsigned replace_byte (unsigned x, int i, unsigned char b);
void ex_2_60()
{
    assert(replace_byte(0x12345678, 2, 0xAB) == 0x12AB5678);
    assert(replace_byte(0x12345678, 0, 0x00) == 0x12345600);
}

int main(void)
{
    ex_2_58();
    ex_2_59();
    ex_2_60();

    return 0;
}
