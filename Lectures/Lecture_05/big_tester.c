#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "4096_t.h"

// Convert hex string to uint64_t array (little-endian)
/* hex string to  4096_t */
uint64_t hex_to_big(char *h, uint64_t *n) {
	size_t l, i = 0, o; /* len, index, offset */
	memset(n, 0, BYTES);
	h += 2;
	l = strlen(h);
	o = l - 16;
	while (l > o) {
		sscanf(h + o, "%lx", &n[i]);
		h[o] = 0;
		i++;
		o -= 16;
	}
	sscanf(h, "%lx", &n[i]);
	return 0;
}
// Prints a big number in big-endian format (MSW first)
uint64_t bigout(uint64_t *n) {
    size_t i = S - 1;
    for (; i < S; i--) {
        printf("%016lx", n[i]);  // Print each 64-bit chunk as a hex string
    }
    printf("\n");
    return 0;
}

// Function to process the requested operation
int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <hexA> <hexB> <operation>\n", argv[0]);
        exit(1);
    }

    uint64_t A[S] = {0}, B[S] = {0}, RESULT[S] = {0};

    hex_to_big(argv[1], A);
    hex_to_big(argv[2], B);

    // Determine the operation requested
    switch (argv[3][0]) {
        case 'A':  // ADD
            bigadd(A, B, RESULT);
            return bigout(RESULT);
        case 'S':  // SUBTRACT
            bigsub(A, B, RESULT);
            return bigout(RESULT);
        case 'M':  // MULTIPLY
            bigmul(A, B, RESULT);
            return bigout(RESULT);
        case 'Q':  // QUOTIENT (bigquo)
            bigquo(A, B, RESULT);
            return bigout(RESULT);
        case 'R':  // REMAINDER (bigrem)
            bigrem(A, B, RESULT);
            return bigout(RESULT);
        default:
            fprintf(stderr, "Invalid operation. Use A (Add), S (Sub), M (Mul), Q (Quo), R (Rem)\n");
            exit(1);
    }
}

