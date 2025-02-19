#include <stdio.h>
#include <stdint.h>

// Generic byte swap function for any integer size
void byte_swap(void *value, size_t length) {
    uint8_t swap, *alias = (uint8_t *)value;
    for (size_t i = 0; i < length / 2; i++) {
        swap = alias[i];
        alias[i] = alias[length - i - 1];
        alias[length - i - 1] = swap;
    }
}

// Macro to call `byte_swap()` with any integer type
#define HTON(n) (byte_swap(&(n), sizeof(n)), n)

int main() {
    uint16_t a = 0x1234;
    uint32_t b = 0x12345678;
    uint64_t c = 0x123456789ABCDEF0;

    printf("HTON(0x%04x) -> 0x%04x\n", a, HTON(a));
    printf("HTON(0x%08x) -> 0x%08x\n", b, HTON(b));
    printf("HTON(0x%016llx) -> 0x%016llx\n", c, HTON(c));

    return 0;
}

