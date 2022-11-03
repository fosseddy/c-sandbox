#include <stdio.h>

typedef unsigned char byte;

void show_bytes(byte *p, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%2x ", p[i]);
    }
    printf("\n");
}

int main(void)
{
    int x = 0x87654321;
    int len = sizeof(int);

    int r1 = x & 0xFF;
    int r2 = x ^ ~0xFF;
    int r3 = x | 0xFF;

    show_bytes((byte *) &r1, len);
    show_bytes((byte *) &r2, len);
    show_bytes((byte *) &r3, len);

    return 0;
}
