#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>     // for dirname
#include <limits.h>     // for PATH_MAX
#include <openssl/sha.h>

#define HASH_SIZE 32
#define MAX_TRANSACTIONS 256
#define MAX_TX_SIZE 2048  // Enough for text + signature

void hash_pair(const uint8_t *left, const uint8_t *right, uint8_t *out) {
    uint8_t buffer[HASH_SIZE * 2];
    memcpy(buffer, left, HASH_SIZE);
    memcpy(buffer + HASH_SIZE, right, HASH_SIZE);
    SHA256(buffer, HASH_SIZE * 2, out);
}

uint8_t* build_merkle_tree(uint8_t tx_hashes[][HASH_SIZE], int tx_count, int *tree_size_out) {
    int level_size = tx_count;
    int total_nodes = 0;
    uint8_t (*tree)[HASH_SIZE] = malloc(sizeof(uint8_t[MAX_TRANSACTIONS * 2][HASH_SIZE]));
    if (!tree) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    memcpy(tree, tx_hashes, tx_count * HASH_SIZE);
    total_nodes += tx_count;

    int offset = 0;
    while (level_size > 1) {
        for (int i = 0; i < level_size; i += 2) {
            uint8_t *left = tree[offset + i];
            uint8_t *right = (i + 1 < level_size) ? tree[offset + i + 1] : left;
            hash_pair(left, right, tree[total_nodes]);
            total_nodes++;
        }
        offset += level_size;
        level_size = (level_size + 1) / 2;
    }

    *tree_size_out = total_nodes;
    return (uint8_t*)tree;
}

void write_merkle_tree_to_same_dir(const char *input_path, uint8_t *tree, int tree_size) {
    char path_buf[PATH_MAX];
    strncpy(path_buf, input_path, PATH_MAX - 1);
    path_buf[PATH_MAX - 1] = '\0';

    char *dir = dirname(path_buf);
    char out_path[PATH_MAX];
    snprintf(out_path, sizeof(out_path), "%s/merkle_tree.bin", dir);

    FILE *f = fopen(out_path, "wb");
    if (!f) {
        perror("fopen");
        exit(1);
    }

    fwrite(&tree_size, sizeof(int), 1, f);
    fwrite(tree, HASH_SIZE, tree_size, f);
    fclose(f);

    printf("✅ Merkle tree written to %s\n", out_path);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block_transactions/b_xx/parsed_transactions.bin\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    uint32_t tx_count;
    fread(&tx_count, sizeof(uint32_t), 1, f);
    if (tx_count > MAX_TRANSACTIONS) {
        fprintf(stderr, "Too many transactions\n");
        return 1;
    }

    uint8_t tx_hashes[MAX_TRANSACTIONS][HASH_SIZE];
    for (uint32_t i = 0; i < tx_count; i++) {
        uint32_t text_len, sig_len;
        fread(&text_len, sizeof(uint32_t), 1, f);
        uint8_t *text = malloc(text_len);
        fread(text, 1, text_len, f);

        fread(&sig_len, sizeof(uint32_t), 1, f);
        uint8_t *sig = malloc(sig_len);
        fread(sig, 1, sig_len, f);

        uint8_t tx_blob[MAX_TX_SIZE];
        memcpy(tx_blob, text, text_len);
        memcpy(tx_blob + text_len, sig, sig_len);

        SHA256(tx_blob, text_len + sig_len, tx_hashes[i]);

        free(text);
        free(sig);
    }
    fclose(f);

    int tree_nodes;
    uint8_t *tree = build_merkle_tree(tx_hashes, tx_count, &tree_nodes);

    printf("Merkle root: ");
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("%02x", tree[(tree_nodes - 1) * HASH_SIZE + i]);
    }
    printf("\n");

    write_merkle_tree_to_same_dir(argv[1], tree, tree_nodes);
    free(tree);
    return 0;
}
