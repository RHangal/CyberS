#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HASH_SIZE 32
#define DIFFICULTY_PREFIX 0x00

void sha256(const uint8_t *data, size_t len, uint8_t *out) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, data, len);
    SHA256_Final(out, &ctx);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block.bin\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    // Read entire block into buffer
    uint8_t *block_data = malloc(file_size);
    if (!block_data) {
        fprintf(stderr, "malloc failed\n");
        fclose(f);
        return 1;
    }

    fread(block_data, 1, file_size, f);
    fclose(f);

    // Hash full block (including nonce at the end)
    uint8_t hash[HASH_SIZE];
    sha256(block_data, file_size, hash);
    free(block_data);

    // Check difficulty
    if (hash[0] == DIFFICULTY_PREFIX) {
        printf("✅ Block is valid. Hash: ");
    } else {
        printf("❌ Invalid block! Hash does not meet difficulty.\nHash: ");
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return hash[0] == DIFFICULTY_PREFIX ? 0 : 1;
}
