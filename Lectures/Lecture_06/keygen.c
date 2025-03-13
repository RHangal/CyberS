#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define E 65537  // Public exponent

// Function to check if a number is prime
int is_prime(uint16_t num) {
    if (num < 2) return 0;
    for (uint16_t i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Function to generate a random 16-bit prime number
uint16_t generate_prime() {
    uint16_t num;
    do {
        num = (rand() % (0x10000 - 0x8000)) + 0x8000;  // 16-bit prime in [0x8000, 0x10000]
    } while (!is_prime(num));
    return num;
}

// Function to calculate GCD using the Euclidean algorithm
uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Function to compute LCM (Least Common Multiple)
uint64_t lcm(uint64_t a, uint64_t b) {
    return (a / gcd(a, b)) * b;
}

// Extended Euclidean Algorithm to find modular inverse of e mod λ(n)
uint64_t mod_inverse(uint64_t e, uint64_t lambda) {
    int64_t t = 0, new_t = 1;
    int64_t r = lambda, new_r = e;

    while (new_r != 0) {
        uint64_t quotient = r / new_r;
        int64_t temp = new_t;
        new_t = t - quotient * new_t;
        t = temp;
        temp = new_r;
        new_r = r - quotient * new_r;
        r = temp;
    }

    if (t < 0) t += lambda;  // Ensure d is positive
    return (uint64_t)t;
}

// Function for modular exponentiation (used for encryption & decryption)
uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to write keys to files
void write_keys(uint64_t n, uint64_t d) {
    FILE *priv = fopen("unsafe.bad", "w");
    FILE *pub = fopen("unsafe.pub", "w");

    if (!priv || !pub) {
        printf("Error: Unable to open output files.\n");
        exit(1);
    }

    // Write private key
    fprintf(priv, "-----BEGIN UNSAFE PRIVATE KEY-----\n");
    fprintf(priv, "%llx\n%llx\n%llx\n", n, (uint64_t)E, d);
    fprintf(priv, "-----END UNSAFE PRIVATE KEY-----\n");
    fclose(priv);

    // Write public key
    fprintf(pub, "-----BEGIN UNSAFE PUBLIC KEY-----\n");
    fprintf(pub, "%llx\n%llx\n", n, (uint64_t)E);
    fprintf(pub, "-----END UNSAFE PUBLIC KEY-----\n");
    fclose(pub);
}

int main() {
    srand(time(NULL));  // Seed random number generator

    // Generate two 16-bit prime numbers
    uint16_t p = generate_prime();
    uint16_t q = generate_prime();

    // Ensure distinct primes
    while (p == q) {
        q = generate_prime();
    }

    uint64_t n = (uint64_t)p * q;       // Compute n
    uint64_t lambda = lcm(p - 1, q - 1); // Compute λ(n)
    uint64_t d = mod_inverse(E, lambda); // Compute d (private exponent)

    // Write keys to files
    write_keys(n, d);

    printf("Key generation complete!\n");
    printf("Public key saved in unsafe.pub\n");
    printf("Private key saved in unsafe.bad\n");

    return 0;
}

