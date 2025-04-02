#include "../4096_t/4096_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>


/* print the big value as a string */
void seebig(uint64_t *a) {
    size_t i;
    for (i = S-1; i < S ; i--) {
        fprintf(stderr, "%016lx ", a[i]); 
        if ((i % 8 == 0 && i)) {
            fprintf(stderr, "\n");
        }       
    }
    fprintf(stderr, "\n\n");
    return;
}

void prigmp(uint64_t *big, uint8_t words) {
    /* populate from buffer */
    mpz_t m;
    FILE *fp = fopen("/dev/random", "r");
    memset(big, 0, BYTES);
    fread(big, sizeof(uint64_t), words, fp);
    fclose(fp);
    mpz_init(m);
    mpz_import(m, S, -1, sizeof(uint64_t), 0, 0, big);
    mpz_nextprime(m, m);
    mpz_export(big, NULL, -1, sizeof(uint64_t), 0, 0, m);
    mpz_clear(m);
    return;
}

int main() {
        uint64_t bignum[S];
        prigmp(bignum, 32);
        seebig(bignum);
        return 0;
}
