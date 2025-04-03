#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "4096_t.h"

#define MAXBUF 4096

// Helper to read a 4096_t value from hex string in file
void fromhex(FILE *fp, uint64_t *n) {
    char buf[MAXBUF];
    memset(n, 0, BYTES);
    if (!fgets(buf, MAXBUF, fp)) return;

    // Strip newline if needed
    char *newline = strchr(buf, '\n');
    if (newline) *newline = '\0';

    size_t len = strlen(buf);
    size_t chunks = len / 16 + (len % 16 != 0);
    for (size_t i = 0; i < chunks; i++) {
        char temp[17] = {0};
        size_t start = len > 16 * (i + 1) ? len - 16 * (i + 1) : 0;
        size_t count = len - start - i * 16;
        strncpy(temp, buf + start, count);
        sscanf(temp, "%lx", &n[i]);
    }
}

// Write 4096_t as hex to file
void tohex(FILE *fp, uint64_t *n) {
    int start = S - 1;
    while (start > 0 && n[start] == 0) start--;
    for (int i = start; i >= 0; i--) {
        fprintf(fp, "%016lx", n[i]);
    }
    fprintf(fp, "\n");
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s -e|-d infile outfile\n", argv[0]);
        return 1;
    }

    int decrypt = argv[1][1] == 'd';

    FILE *keyfp = fopen(decrypt ? "unsafe.bad" : "unsafe.pub", "r");
    if (!keyfp) {
        perror("key file");
        return 1;
    }

    char line[MAXBUF];
    fgets(line, MAXBUF, keyfp); // Skip header
    uint64_t n[S], e[S], d[S];
    fromhex(keyfp, n);
    fromhex(keyfp, decrypt ? d : e);
    fclose(keyfp);

    FILE *in = fopen(argv[2], "r");
    FILE *out = fopen(argv[3], "w");
    if (!in || !out) {
        perror("file");
        return 1;
    }

    char buf[MAXBUF];
    while (fgets(buf, MAXBUF, in)) {
        uint64_t m[S] = {0};
        uint64_t c[S] = {0};
        size_t len = strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';

        // Convert input string to a big number (basic)
        sscanf(buf, "%lx", &m[0]);

        // Do modular exponentiation
        bigpowmod(m, decrypt ? d : e, n, c);

        // Output result as hex
        tohex(out, c);
    }

    fclose(in);
    fclose(out);
    return 0;
}

