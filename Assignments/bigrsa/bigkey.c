#include "./bigrsa.h"
#include <stdio.h>

int main() {
    int iterator = 0;
    uint64_t prime_p[S], prime_q[S], lambda[S], public_n[S], secret_d[S],
             encryptor = STD_ENCRYPTOR;
    FILE *output_file;
    do {
        big_prime(prime_p, RANDOM_WORDS);
        big_prime(prime_q, RANDOM_WORDS);
        bigmul(prime_p, prime_q, public_n);
        prime_p[0] -= 1;
        prime_q[0] -= 1;
        /* If we generate primes that are the same or too big, big_lcm will return 1
         */
        iterator++;
    } while (iterator < MAX_FIND_PRIMES_ITER &&
             (big_lcm(prime_p, prime_q, lambda) ||
              big_find_d(encryptor, lambda, secret_d)
              /*|| get_msb(secret_d) < 3000 */));
    if (iterator == MAX_FIND_PRIMES_ITER) {
        fprintf(stderr,
                "bigkey.o: Failed to find compliant values after %d tries.\n",
                MAX_FIND_PRIMES_ITER);
        return 1;
    }

    output_file = fopen(FILENAME_PRIVATE, "w");
    if (write_key(output_file, MODE_PRIVATE, public_n, encryptor, secret_d)) {
        fprintf(stderr, "bigkey.o: Failed to output to file %s.\n",
                FILENAME_PRIVATE);
        return 1;
    }
    output_file = fopen(FILENAME_PUBLIC, "w");
    if (write_key(output_file, MODE_PUBLIC, public_n, encryptor, NULL)) {
        fprintf(stderr, "bigkey.o: Failed to output to file %s.\n",
                FILENAME_PUBLIC);
        return 1;
    }
    return 0;
}
