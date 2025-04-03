#include "../4096_t/4096_t.h"
#include "../ops_ui/ops_ui.h"
#include "bigutils.h"
#include <stdio.h>
#include <string.h>

#define PUBKEY "unsafe.pub"
#define PRIVKEY "unsafe.bad"
#define E_CONST 0x10001

void toline(FILE *fp, uint64_t *a) {
    for (int i = S - 1; i >= 0; i--) {
        fprintf(fp, "%016lx", a[i]);
    }
    fprintf(fp, "\n");
}

int main() {
    uint64_t p[S], q[S], n[S], phi[S];
    uint64_t p1[S] = {0}, q1[S] = {0}, d[S] = {0};
    uint64_t e[S] = {0}, g[S] = {0}, x[S] = {0};

    printf("Generating 2048-bit primes...\n");
    prigmp(p, S / 2);
    prigmp(q, S / 2);

    printf("Computing n = p * q...\n");
    bigmul(p, q, n);

    printf("Computing phi = lcm(p-1, q-1)...\n");
    sub_ui(p, 1, p1);
    sub_ui(q, 1, q1);
    biglcm(p1, q1, phi);

    printf("Setting e = 0x10001 and computing d = e^{-1} mod phi...\n");
    e[0] = E_CONST;
    bigmmi(e, phi, d);

    printf("Writing %s and %s...\n", PUBKEY, PRIVKEY);

    // Write .bad
    FILE *bad = fopen(PRIVKEY, "w");
    if (!bad) {
        fprintf(stderr, "Error opening %s\n", PRIVKEY);
        return 1;
    }
    fprintf(bad, "-----BEGIN UNSAFE PRIVATE KEY-----\n");
    toline(bad, n);
    fprintf(bad, "%016lx\n", E_CONST);
    toline(bad, d);
    fprintf(bad, "-----END UNSAFE PRIVATE KEY-----\n");
    fclose(bad);

    // Write .pub
    FILE *pub = fopen(PUBKEY, "w");
    if (!pub) {
        fprintf(stderr, "Error opening %s\n", PUBKEY);
        return 1;
    }
    fprintf(pub, "-----BEGIN UNSAFE PUBLIC KEY-----\n");
    toline(pub, n);
    fprintf(pub, "%016lx\n", E_CONST);
    fprintf(pub, "-----END UNSAFE PUBLIC KEY-----\n");
    fclose(pub);

    printf("Key generation complete.\n");
    return 0;
}

