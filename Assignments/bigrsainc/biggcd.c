#include "../4096_t/4096_t.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>



void seebig(uint64_t *a) {
    for (size_t i = S - 1; i < S; i--) {
        printf("%016lx ", a[i]);
        if (i % 8 == 0 && i != 0) printf("\n");
    }
    printf("\n\n");
}


void prigmp(uint64_t *big, uint8_t words) {
    mpz_t m;
    FILE *fp = fopen("/dev/random", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open /dev/random\n");
        exit(1);
    }
    memset(big, 0, BYTES);
    fread(big, sizeof(uint64_t), words, fp);
    fclose(fp);
    mpz_init(m);
    mpz_import(m, S, -1, sizeof(uint64_t), 0, 0, big);
    mpz_nextprime(m, m);
    mpz_export(big, NULL, -1, sizeof(uint64_t), 0, 0, m);
    mpz_clear(m);
}

void biggcd(uint64_t *in_a, uint64_t *in_b, uint64_t *g) {
    uint64_t temp[S] = {0};
    uint64_t rem[S] = {0};

    memcpy(temp, in_a, BYTES);

    while (bigcmp(in_b, (uint64_t[S]){0}) != 0) {
        bigrem(temp, in_b, rem);
        memcpy(temp, in_b, BYTES);
        memcpy(in_b, rem, BYTES);
    }

    memcpy(g, temp, BYTES);
}

int main() {
    uint64_t p[S] = {0}, q[S] = {0}, g[S] = {0};

    printf("Generating primes...\n");
    prigmp(p, 32);
    prigmp(q, 32);

    printf("Computing GCD...\n");
    biggcd(p, q, g);

    printf("GCD(p, q):\n");
    seebig(g);

    return 0;
}

