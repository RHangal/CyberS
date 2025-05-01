#include "bigrsa.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    short mode = MODE_PUBLIC;
    char mode_char /*, *input_filename, *output_filename*/;
    uint64_t public_n[S], transcryptor[S], input_file[S], out[S];
    memset(transcryptor, 0, BYTES);
    FILE *file_handler;
    if (argc != 4) {
        fprintf(stderr, "Incorrect number of arguments\n%s\n", STR_USAGE);
        return 1;
    }
    mode_char = tolower(argv[1][1]);
    if (mode_char != 'e' && mode_char != 'd') {
        fprintf(stderr, "Improper mode %c\n%s\n", mode_char, STR_USAGE);
        return 1;
    }
    if (mode_char == 'd') {
        mode = MODE_PRIVATE;
    }
    /* Read key */
    file_handler =
        fopen(mode == MODE_PUBLIC ? FILENAME_PUBLIC : FILENAME_PRIVATE, "r");
    if (file_handler == NULL) {
        fprintf(stderr, "Could not read file %s\n%s\n",
                mode == MODE_PUBLIC ? FILENAME_PUBLIC : FILENAME_PRIVATE,
                STR_USAGE);
        return 1;
    }
    /* transcryptor will become encryptor in MODE_PUBLIC, otherwise it will become
     * secret_d */
    if (read_key(file_handler, mode, public_n, transcryptor)) {
        fprintf(stderr, "Failed to read key in %s\n",
                mode == MODE_PUBLIC ? FILENAME_PUBLIC : FILENAME_PRIVATE);
        return 1;
    }
    file_handler = fopen(argv[2], "r");
    if (file_handler == NULL) {
        fprintf(stderr, "Could not read file %s\n%s\n", argv[2], STR_USAGE);
        return 1;
    }
    input_read(file_handler, input_file);
    if (strcmp(argv[3], "STDOUT") == 0) {
        file_handler = stdout;
    } else {
        file_handler = fopen(argv[3], "w");
    }
    /* It's time for some big math now */
    if (big_modexp(input_file, transcryptor, public_n, out)) {
        fprintf(stderr, "Could not perform modular exponentiation\n");
        return 1;
    }
    output_write(file_handler, out, mode == MODE_PRIVATE);
    return 0;
}
