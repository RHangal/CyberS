#include <stdio.h>

void printb(unsigned char value) {
    // Number of bits in a char (typically 8)
    int num_bits = sizeof(unsigned char) * 8;

    // Print each bit, starting from the most significant bit (MSB)
    for (int i = num_bits - 1; i >= 0; i--) {
        // Use bitwise AND and shift to extract each bit
        printf("%d", (value >> i) & 1);
    }
    printf("\n"); // Add a newline for readability
}

int main() {
    unsigned char test_value;

    // Test with different values
    for (int i = 0; i <= 10; i++) {
        test_value = (unsigned char)i;
        printf("Binary of %d: ", test_value);
        printb(test_value);
    }

    return 0;
}

