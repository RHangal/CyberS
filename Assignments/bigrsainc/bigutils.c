#include "bigutils.h"
#include <gmp.h>
#include <string.h>
#include <stdlib.h>

void seebig(uint64_t *a) {
    for (size_t i = S - 1; i < S; i--) {
        printf("%016llx ", (unsigned long long)a[i]);
        if (i % 8 == 0 && i != 0) printf("\n");
    }
    printf("\n\n");
}

void prigmp(uint64_t *big, uint8_t words) {
    mpz_t m;
    FILE *fp = fopen("/dev/urandom", "r");
    if (!fp) {
        perror("Failed to open /dev/urandom");
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

int is_zero(uint64_t *n) {
    for (size_t i = 0; i < S; i++) {
        if (n[i] != 0) return 0;
    }
    return 1;
}


void bigmmi(uint64_t *e, uint64_t *phi, uint64_t *out) {
    uint64_t g[S], x[S];

    biggcd(e, phi, g, x);  // extended GCD to get modular inverse

    // If x is negative, add phi
    if ((int64_t)x[S - 1] < 0) {
        bigadd(x, phi, out);
    } else {
        memcpy(out, x, BYTES);
    }
}



void biggcd(uint64_t *a, uint64_t *b, uint64_t *g, uint64_t *x) {
    uint64_t r0[S], r1[S], s0[S], s1[S];
    uint64_t q[S], t[S], prod[S];
    uint64_t temp[S];
    int sign_s0 = 0, sign_s1 = 1;

    memcpy(r0, a, BYTES);
    memcpy(r1, b, BYTES);
    memset(s0, 0, BYTES);
    memset(s1, 0, BYTES);
    s0[0] = 1;

    while (!is_zero(r1)) {
        bigquo(r0, r1, q);

        // r0 - q * r1 → temp
        bigmul(q, r1, prod);
        bigsub(r0, prod, temp);

        memcpy(r0, r1, BYTES);
        memcpy(r1, temp, BYTES);

        // s0 - q * s1 → temp
        bigmul(q, s1, prod);
        if (bigcmp(s0, prod) < 0) {
            bigsub(prod, s0, temp);
            sign_s1 = !sign_s0;
        } else {
            bigsub(s0, prod, temp);
            sign_s1 = sign_s0;
        }

        memcpy(s0, s1, BYTES);
        memcpy(s1, temp, BYTES);
        sign_s0 = sign_s1;
    }

    if (x) {
        if (sign_s0) {
            bigsub(b, s0, x);
        } else {
            memcpy(x, s0, BYTES);
        }
    }

    memcpy(g, r0, BYTES);
}

void biglcm(uint64_t *a, uint64_t *b, uint64_t *out) {
    uint64_t g[S], tmp[S];
    biggcd(a, b, g, NULL);        // g = gcd(a, b)
    bigmul(a, b, tmp);            // tmp = a * b
    bigquo(tmp, g, out);          // out = tmp / g
}

