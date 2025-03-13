#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define E ((uint64_t)65537) 


int is_prime(uint16_t num) {
    uint16_t i;
    if (num < 2) return 0;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}


uint16_t generate_prime() {
    uint16_t num;
    do {
        num = (rand() % (0x10000 - 0x8000)) + 0x8000; 
    } while (!is_prime(num));
    return num;
}


uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


uint64_t lcm(uint64_t a, uint64_t b) {
    return (a / gcd(a, b)) * b;
}


uint64_t mod_inverse(uint64_t e, uint64_t lambda) {
    int64_t t = 0, new_t = 1;
    int64_t r = lambda, new_r = e;
    uint64_t quotient, temp;

    while (new_r != 0) {
        quotient = r / new_r;
        temp = new_t;
        new_t = t - quotient * new_t;
        t = temp;
        temp = new_r;
        new_r = r - quotient * new_r;
        r = temp;
    }

    if (t < 0) t += lambda; 
    return (uint64_t)t;
}


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


void write_keys(uint64_t n, uint64_t d) {
    FILE *priv, *pub;

    priv = fopen("unsafe.bad", "w");
    pub = fopen("unsafe.pub", "w");

    if (!priv || !pub) {
        printf("Error: Unable to open output files.\n");
        exit(1);
    }

   
    fprintf(priv, "-----BEGIN UNSAFE PRIVATE KEY-----\n");
    fprintf(priv, "%lx\n%lx\n%lx\n", (unsigned long)n, (unsigned long)E, (unsigned long)d);
    fprintf(priv, "-----END UNSAFE PRIVATE KEY-----\n");
    fclose(priv);

    
    fprintf(pub, "-----BEGIN UNSAFE PUBLIC KEY-----\n");
    fprintf(pub, "%lx\n%lx\n", (unsigned long)n, (unsigned long)E);
    fprintf(pub, "-----END UNSAFE PUBLIC KEY-----\n");
    fclose(pub);
}

int main() {
    uint16_t p, q;
    uint64_t n, lambda, d;

    srand(time(NULL)); 

    
    p = generate_prime();
    q = generate_prime();

   
    while (p == q) {
        q = generate_prime();
    }

    n = (uint64_t)p * q;      
    lambda = lcm((uint64_t)(p - 1), (uint64_t)(q - 1));
    d = mod_inverse(E, lambda); 

   
    write_keys(n, d);


    return 0;
}

