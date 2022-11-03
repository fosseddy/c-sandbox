#include <stdio.h>

typedef unsigned char byte;

void show_bytes(byte *p, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%2x ", p[i]);
    }
    printf("\n");
}

int bis(int x, int m) {
    return x | m;
}

int bic(int x, int m) {
    return x ^ m;
}

int main(void)
{
    int x = 0x87654321;
    int len = sizeof(int);

    int r1 = x & 0xFF;
    int r2 = (~x ^ ~x & 0xFF) | (x & 0xFF);
    int r3 = (x ^ x & 0xFF) | (~0 & 0xFF);

    show_bytes((byte *) &r1, len);
    show_bytes((byte *) &r2, len);
    show_bytes((byte *) &r3, len);

    int r4 = bis(0x00000000, 0x00ff00ff);
    int r5 = bic(0xffff00ff, 0x00ff00ff);

    show_bytes((byte *) &r4, len);
    show_bytes((byte *) &r5, len);

    return 0;
}
