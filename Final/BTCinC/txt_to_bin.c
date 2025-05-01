#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIG_HEX 480  // 2048-bit RSA = 256 bytes = 512 hex chars

// Convert hex string to byte array
static void hex_to_bytes(const char *hex, uint8_t *out, size_t out_len) {
    for (size_t i = 0; i < out_len; i++) {
        if (sscanf(hex + 2 * i, "%2hhx", &out[i]) != 1) {
            fprintf(stderr, "Invalid hex format at byte %zu\n", i);
            exit(1);
        }
    }
}

// Trim leading and trailing whitespace
char *trim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s block_transaction_XX.txt\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char *text_raw = NULL;
    char sig_hex[MAX_SIG_HEX + 1] = {0};
    size_t sig_hex_len = 0;

    enum { NONE, TEXT, HASH, SIG } state = NONE;

    while ((read = getline(&line, &len, f)) != -1) {
        line[strcspn(line, "\r\n")] = '\0';

        if (strcmp(line, "Transaction:") == 0) { state = TEXT; continue; }
        if (strcmp(line, "Transaction Hash:") == 0) { state = HASH; continue; }
        if (strcmp(line, "Transaction Signature:") == 0) { state = SIG; continue; }

        if (state == TEXT && !text_raw && strlen(line) > 0) {
            text_raw = strdup(line);
        } else if (state == SIG) {
            char *trimmed = trim(line);
            size_t this_len = strlen(trimmed);
            if (sig_hex_len + this_len > MAX_SIG_HEX) {
                fprintf(stderr, "Signature too long in %s\n", argv[1]);
                fclose(f);
                free(line);
                free(text_raw);
                return 1;
            }
            memcpy(sig_hex + sig_hex_len, trimmed, this_len);
            sig_hex_len += this_len;
        }
    }

    fclose(f);
    free(line);

    if (!text_raw || sig_hex_len != MAX_SIG_HEX) {
        fprintf(stderr, "❌ Error parsing %s\n", argv[1]);
        fprintf(stderr, "text_raw: %s\n", text_raw ? text_raw : "(null)");
        fprintf(stderr, "sig_hex_len: %zu (expected %d)\n", sig_hex_len, MAX_SIG_HEX);
        return 1;
    }

    uint32_t text_len = strlen(text_raw);
    uint32_t sig_len = sig_hex_len / 2;
    uint8_t *sig_bin = malloc(sig_len);
    if (!sig_bin) {
        perror("malloc");
        free(text_raw);
        return 1;
    }

    hex_to_bytes(sig_hex, sig_bin, sig_len);

    fwrite(&text_len, sizeof(uint32_t), 1, stdout);
    fwrite(text_raw, 1, text_len, stdout);
    fwrite(&sig_len, sizeof(uint32_t), 1, stdout);
    fwrite(sig_bin, 1, sig_len, stdout);

    free(text_raw);
    free(sig_bin);

    return 0;
}
