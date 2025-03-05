#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// SHA-256 Constants
uint32_t k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
    0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
    0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
    0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
    0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
    0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
    0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
    0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
    0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

// Initial hash values
uint32_t h[8] = {
    0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
    0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
};


void byte_swap(void *value, size_t length) {
    uint8_t swap, *alias = (uint8_t *)value;
    for (size_t i = 0; i < length / 2; i++) {
        swap = alias[i];
        alias[i] = alias[length - i - 1];
        alias[length - i - 1] = swap;
    }
}

#define HTON(n) (byte_swap(&(n), sizeof(n)), n)

#define ROTR(x, n) ( (x >> n) | (x << (32 - n)))

#define s0(x) (ROTR(x,7) ^ ROTR(x,18) ^ (x >> 3))
#define s1(x) (ROTR(x,17) ^ ROTR(x,19) ^ (x >> 10))

#define S0(a) (ROTR(a,2) ^ ROTR(a,13) ^ ROTR(a,22)) 
#define S1(e) (ROTR(e,6) ^ ROTR(e,11) ^ ROTR(e,25))

#define MAJ(a,b,c) ( (a & b) ^ (a & c) ^ (b & c) )
#define CH(e,f,g) ((e & f) ^ ((~e) & g))

void sha256_hash(const uint8_t *chunk) {
    uint32_t W[64]; 
    uint32_t a, b, c, d, e, f, g, h_temp, temp1, temp2;
 
    for (size_t i = 0; i < 16; i++) {
        W[i] = HTON(((uint32_t *)chunk)[i]);
    }
    
    for (size_t i=16; i<64; i++) {
        W[i] = s1(W[i-2]) + W[i-7] + s0(W[i-15]) + W[i-16];
    }

    a = h[0], b = h[1], c = h[2], d = h[3];
    e = h[4], f = h[5], g = h[6], h_temp = h[7];
     
    for(size_t i=0; i<64; i++) {
        temp1 = h_temp + S1(e) + CH(e,f,g) + k[i] + W[i];
        temp2 = S0(a) + MAJ(a,b,c);
        
        h_temp = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }
    
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += h_temp;


}

void padded_message(const uint8_t *input, size_t length, size_t total_length, uint8_t *buffer) {
    memcpy(buffer,input,length);
    buffer[length++] = 0x80;
    size_t pad_length = ( length % 64 <= 56) ? 64 : 128;
    while ((length % pad_length) != pad_length-8) {
        buffer[length++]=0x00;
    }
    uint64_t bit_length = total_length * 8;
    bit_length = HTON(bit_length);
    memcpy(&buffer[length], &bit_length, 8);

   }


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "No filename provided, exiting...\n");
        exit(1);
    }
    uint8_t m[64];
    uint8_t m1[128];
    size_t i,length = 0;
    size_t total_length = 0;
    FILE *file = fopen(argv[1], "rb");

    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        exit(1);
    }

    while ((length = fread(m,1,64,file)) == 64) {
        total_length+=length;
        sha256_hash(m);
    }
    total_length+=length;
    padded_message(m,length,total_length,m1);


    sha256_hash(m1);
    if (length % 64 > 56) {
        sha256_hash(m1 + 64);
    }

   fclose(file);

    for (i = 0; i<8; i++) {
        printf("%08x", h[i]);
    }
    printf("  %s\n",argv[1]);

    return 0;
}

