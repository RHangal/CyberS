#include "../4096_t/4096_t.h"
#include "../ops_ui/ops_ui.h"
#include "bigutils.h"
#include <stdio.h>
#include <string.h>

int main() {
    uint64_t a[S], b[S], g[S], x[S], lcm[S];

    printf("Generating random 2048-bit prime p...\n");
    prigmp(a, 32);
    seebig(a);

    printf("Generating random 2048-bit prime q...\n");
    prigmp(b, 32);
    seebig(b);

    printf("Computing GCD(p, q)...\n");
    biggcd(a, b, g, x);
    seebig(g);

    printf("Computing LCM(p-1, q-1)...\n");
    uint64_t p1[S] = {0}, q1[S] = {0};
    sub_ui(a, 1, p1);
    sub_ui(b, 1, q1);
    biglcm(p1, q1, lcm);
    seebig(lcm);

    return 0;
}

