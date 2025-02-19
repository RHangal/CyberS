/* endian.c - &exists; C99+ <endian.h> be advised */

/*
gcc endian.c -O2 -o endian
*/

#include <stdio.h>  /* fileio   */
#include <stdint.h> /* uint32_t */

/* Optionally - endianness compatability.  */
#include <arpa/inet.h> /* htonl */
//#include <endian.h> /* htobe32 */

uint32_t my_htonl(uint32_t n) {
    uint8_t swap, *alias = (uint8_t *)&n;
    size_t index;
    printf("n = 0x%x\n", n);            // Prints the full 32-bit value
    printf("alias = %p\n", (void *)alias);  // Prints the memory address of n
    printf("*alias = 0x%x\n", *alias); // Prints the first byte (least significant byte in little-endian)

    swap = alias[0];
    alias[0] = alias[3];
    alias[3] = swap;
    swap = alias[1];
    alias[1] = alias[2];
    alias[2] = swap;
    printf("*alias = 0x%x\n", *alias); // Prints the first byte (least significant byte in little-endian)
    return n;
}

uint64_t my64_htonl(uint64_t n) {
    uint8_t swap, *alias = (uint8_t *)&n;
    size_t index;
    size_t length = sizeof(n);
    if (length != 8) {
        fprintf(stderr,"not 64 bit");
        return 0;    
    }
    for (index=0; index < length/2; index++) {
        swap = alias[index];
        alias[index] = alias[length-index-1];
        alias[length-index-1] = swap;
    }
    return n;
}

int main() {
    uint32_t test = 0x1234ABCD;
    uint64_t testtwo = 0x123456789ABCDEF0;
    printf("%08X\n%08X\n%08X\n%16llX\n",
           test,
           htonl(test),
          // htobe32(test),
           my_htonl(test),
           my64_htonl(testtwo)
          );
    return 0;
}
