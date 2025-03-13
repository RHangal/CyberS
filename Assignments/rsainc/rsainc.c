#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int is_little_endian() {
    union {
        uint32_t i;
        char c[4];
    } test;
    test.i = 1;  
    return test.c[0]; 
}


uint64_t modexp(uint64_t m, uint64_t e, uint64_t n) {
    if(e==0){return 1;}
    if(e==1){return m % n;}
    if(e % 2){
        return (m * modexp((m*m) % n, e/2, n)) % n;
    }
    return modexp((m*m) % n, e/2, n);
}

/*
uint64_t modexp(uint64_t m, uint64_t e, uint64_t n) {
    uint64_t result = 1;
    m = m % n;
    
    while (e > 0) {
        if (e & 1) {  // If e is odd
            result = (result * m) % n;
        }
        e >>= 1;  // Divide e by 2
        m = (m * m) % n;
    }
    return result;
}
*/

int getKey(char flag, uint64_t* e, uint64_t* n) {
    FILE* file;
    size_t lines_to_read = (flag == 'e') ? 2 : 3;
    size_t i = 0;
    char buffer[64];
    
    file = fopen(flag == 'e'? "unsafe.pub" : "unsafe.bad", "r"); 
        if(!file) {
            fprintf(stderr, "Error, unable to open file.\n");
            return 1;
        }
    

    while (i <= lines_to_read) {
        if (!fgets(buffer, sizeof(buffer), file)) {
            fprintf(stderr, "Error: Failed to read line %ld.\n", i);
            fclose(file);
            return 1;
        }

        if (i == 1) {  
            *n = strtoul(buffer, NULL, 16);
        } else if (i == lines_to_read) {  
            *e = strtoul(buffer, NULL, 16);
        }

        i++; 
    }

    fclose(file);
    return 0;    
   
}
    


uint64_t from_bytes(unsigned char* s, size_t length) {
    uint64_t res = 0;   
    size_t i;
    
    if(is_little_endian()) {
        for (i = 0; i < length; i++) {
            res |= (uint64_t)s[i] << (i * 8);
        }
    }
    else {
         for (i = 0; i < length; i++) {
            res = (res << 8) | s[i];
        }
    }

    return res;
}


uint64_t read_file_as_int(const char* filename, uint64_t* value) {
    size_t size;
    unsigned char* buffer;
    size_t l;

    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file.\n");
        return 1;
    }

   
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    
    buffer = malloc(size);
    if (!buffer) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return 1;
    }
    l = fread(buffer, 1, size, file);
    if (l != size) {
        if (feof(file)) {
            fprintf(stderr, "Error: Unexpected end of file while reading %s.\n", filename);
    } else if (ferror(file)) {
        fprintf(stderr, "Error: Read error while reading %s.\n", filename);
    }
    free(buffer);
    fclose(file);
    return 1;  
}

   
    *value = from_bytes(buffer, size);
    free(buffer);
    return 0;
}

/* Convert an integer to a 4-byte binary array */
void to_bytes(uint32_t value, unsigned char* buffer) {
    size_t i;
    if (is_little_endian()) {
        for (i = 0; i < 4; i++) {
            buffer[i] = (value >> (i * 8)) & 0xFF;
        }
    } else {
        for (i = 0; i < 4; i++) {
            buffer[3 - i] = (value >> (i * 8)) & 0xFF;
        }
    }
}

/* Write integer to binary file */
int write_binary(const char* filename, uint64_t value) {
    unsigned char buffer[4]; 
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file for writing.\n");
        return 1;
    }

    to_bytes(value, buffer);

    if (fwrite(buffer, 1, 4, file) != 4) {
        fprintf(stderr, "Error: Failed to write data.\n");
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}



int main(int argc, char** argv) {
    char flag;
    uint64_t e, n;
    uint64_t m;
    uint64_t c;
    if (argc != 4) {
        fprintf(stderr, "Usage: %s -e|-d <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    /* Step 1: Determine mode (encryption or decryption) */
    if (strcmp(argv[1], "-e") == 0) {
        flag = 'e';
    } else if (strcmp(argv[1], "-d") == 0) {
        flag = 'd';
    } else {
        fprintf(stderr, "Error: Invalid mode. Use '-e' for encryption or '-d' for decryption.\n");
        return 1;
    }

    /* Step 2: Read key values (n, e/d) from the appropriate file */
    if (getKey(flag, &e, &n) != 0) {
        fprintf(stderr, "Error: Failed to retrieve key values.\n");
        return 1;
    }

    /* Step 3: Read input binary file into an integer (m) */
    if (read_file_as_int(argv[2], &m) != 0) {
    fprintf(stderr, "Error: Failed to read input file.\n");
    return 1;
}
    /* Step 4: Perform modular exponentiation */
    c = modexp(m, e, n);

    /* Step 5: Write the result as 4 bytes into the output file */
    if (write_binary(argv[3], c) != 0) {
        fprintf(stderr, "Error: Failed to write output file.\n");
        return 1;
    }

   /* printf("Success! Output written to %s\n", argv[3]); */
    return 0;
}

