#include <stdio.h>

#define CHOICE(e, f, g) (((e) & (f)) | (~(e) & (g)))
#define MEDIAN(e, f, g) ((e & f) | (f & g) | (g & e))
#define ROTATE(value, shift) (((value) >> (shift)) | ((value) << (32 - (shift))))


int main() {
    /* Various Variables*/
    unsigned a[4] = {0x11110000, 0x11001100, 0x10101010, 0xABCD0123};

    /* CHOICE */
    printf("*Rotates use a decimal shift value, but print in hexadecimal:\n");
    printf("CHOICE(\n%08lX,\n%08lX,\n%08lX) = \n--------\n%08lX\n\n", a[0], a[1], a[2], CHOICE(a[0], a[1], a[2]));
    printf("MEDIAN(\n%08lX,\n%08lX,\n%08lX) = \n--------\n%08lX\n\n", a[0], a[1], a[2], MEDIAN(a[0], a[1], a[2]));
    printf("*Rotates use a decimal shift value, but print in hexadecimal:\n");
    printf("ROTATE(\n%08lX, 04) = \n--------\n%08lX\n\n", a[3], ROTATE(a[3], 4));
    printf("ROTATE(\n%08lX, 08) = \n--------\n%08lX\n\n", a[3], ROTATE(a[3], 8));
    printf("ROTATE(\n%08lX, 12) = \n--------\n%08lX\n\n", a[3], ROTATE(a[3], 12));
    printf("ROTATE(\n%08lX, 02) = \n--------\n%08lX\n\n", 0x1000, ROTATE(0x1000, 2));
    printf("ROTATE(\n%08lX, 30) = \n--------\n%08lX\n\n", 0x1000, ROTATE(0x1000, 30));

    return 0;
}
