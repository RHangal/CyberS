#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    size_t i, l = 0;
    uint8_t m[64]; // 64 bytes = 512 bits
    FILE *file;

    if (argc < 2){
	    fprintf(stderr, "No filename, goodbye");
	    exit(1);
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
	    fprintf(stderr, "failed to open file, goodbye");
	    exit(1);
    }

    do {
	    l = fread(m, 1, 64, file);
	    for (i = 0; i<l; i++) {
		    if(i%4 == 0 && i > 0) {
			    printf(" ");
		    }
		    if (i%16 == 0 && i>0){
			    printf("\n");
		    }
		    printf("%02x", m[i]);
	    }
	    printf("\n\n");
    }while(l==64);

   if (fclose(file) == EOF) {
	   fprintf(stderr, "fclose fails....wut, goodbye");
	   exit(1);
   }

    return 0;
}


