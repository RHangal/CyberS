#include "biggcd.h"
#include "../4096_t/4096_t.h"
#include "../ops_ui/ops_ui.h"
#include <string.h>

// Check if big number is nonzero
int bignul(uint64_t *a) {
    for (size_t i = 0; i < S; i++) {
        if (a[i] != 0) return 1;
    }
    return 0;
}

// Signed subtraction with sign tracking
// Returns sign (0 = positive, 1 = negative)
uint64_t signed_sub(uint64_t *a, uint64_t as, uint64_t *b, uint64_t bs, uint64_t *r) {
    if (as == bs) {
        if (bigmax(b, a)) {
            bigsub(b, a, r);
            return !as;
        } else {
            bigsub(a, b, r);
            return as;
        }
    } else {
        bigadd(a, b, r);
        return as;
    }
}

// Extended Euclidean algorithm (unsigned big ints with manual sign tracking)
void biggcd(uint64_t *in_a, uint64_t *in_b, uint64_t *r, uint64_t *x) {
    uint64_t a[S], b[S], c[S], d[S], q[S], t[S], p[S], *rs[2][2];
    uint64_t qs, ts, ps, i, signs[2][2];

    memcpy(a, in_a, BYTES);
    memcpy(b, in_b, BYTES);
    memset(c, 0, BYTES);
    memset(d, 0, BYTES);
    c[0] = 1;

    rs[0][0] = a;
    rs[0][1] = b;
    rs[1][0] = c;
    rs[1][1] = d;

    signs[0][0] = 0;
    signs[0][1] = 0;
    signs[1][0] = 0;
    signs[1][1] = 0;

    while (bignul(rs[0][1])) {
        bigquo(rs[0][0], rs[0][1], q);
        qs = signs[0][0] != signs[0][1];

        for (i = 0; i < 2; i++) {
            memcpy(t, rs[i][1], BYTES);
            ts = signs[i][1];

            bigmul(q, rs[i][1], p);
            ps = qs != signs[i][1];

            signs[i][1] = signed_sub(rs[i][0], signs[i][0], p, ps, rs[i][1]);
            memcpy(rs[i][0], t, BYTES);
            signs[i][0] = ts;
        }
    }

    if (x) {
        if (signs[1][0]) {
            bigsub(in_b, rs[1][0], x);  // Convert to positive mod inverse
        } else {
            memcpy(x, rs[1][0], BYTES);
        }
    }

    memcpy(r, rs[0][0], BYTES);
}

