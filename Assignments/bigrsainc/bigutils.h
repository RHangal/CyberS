#ifndef _BIGUTILS_H
#define _BIGUTILS_H

#include "../4096_t/4096_t.h"
#include "../ops_ui/ops_ui.h"
#include <stdint.h>
#include <stdio.h>

/* Debugging: Print a big 4096-bit number */
void seebig(uint64_t *a);

/* Random prime generator using GMP */
void prigmp(uint64_t *big, uint8_t words);

/* Extended Euclidean Algorithm to compute gcd and modular inverse */
void biggcd(uint64_t *a, uint64_t *b, uint64_t *g, uint64_t *x);

/* Least Common Multiple using gcd(a,b) */
void biglcm(uint64_t *a, uint64_t *b, uint64_t *out);

void bigmmi(uint64_t *e, uint64_t *phi, uint64_t *out);


/* Check if a big number is zero */
int is_zero(uint64_t *n);

#endif /* _BIGUTILS_H */

