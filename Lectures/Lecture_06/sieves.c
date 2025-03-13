#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000000

uint64_t sqrtBinarySearch(uint64_t n) {
    if (n == 0 || n == 1) return n;

    uint64_t left = 1, right = n, ans = 0;

    while (left <= right) {
        uint64_t mid = left + (right - left) / 2;
        uint64_t mid_squared = mid * mid;

        if (mid_squared == n) return mid;  
        if (mid_squared < n) {
            left = mid + 1;
            ans = mid;  
        } else {
            right = mid - 1;
        }
    }

    return ans;  
}

int sievesOfEratosthenes(uint8_t** arr, uint64_t n) {
    uint64_t p, i, m, maxCheck;
    m = sqrtBinarySearch(n);
    *arr = (uint8_t*)calloc(n,sizeof(uint8_t));
    if(!arr) {
        fprintf(stderr, "Error: Memory allocation failed, Exiting\n");
        return 1;
    }
    
    for(p=2; p <=m; p++) {
        if(!(*arr)[p-1]) {
            maxCheck = UINT64_MAX - p;
            for(i = p*p; i<=n && i <= maxCheck; i+=p) {
               (*arr)[i-1] = 1; 
            }
        }
    }

    return 0;

}

void printPrimes(uint8_t* primeArr, uint64_t n) {
    char buffer[BUFFER_SIZE]; 
    int index = 0;

    for (uint64_t i = 2; i <= n; i++) {
        if (!primeArr[i - 1]) {  
            index += sprintf(buffer + index, "0x%llx ", i); 

           
            if (index >= BUFFER_SIZE - 50) {  
                fwrite(buffer, 1, index, stdout);
                index = 0;
            }
        }
    }

   
    if (index > 0) {
        fwrite(buffer, 1, index, stdout);
    }
}

int main(int argc, char** argv) {
    uint8_t* primeArr;
    const char *str;
    uint64_t result;
    uint8_t digit;
    uint64_t i;
     if (argc < 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    str = argv[1];
    result = 0;

    while (*str) {
        if (!isdigit(*str)) { 
            fprintf(stderr, "Error: Invalid input, Must be a positive integer\n");
            return 1;
        }

        digit = *str - '0';

       
        if (result > (UINT64_MAX / 10) || 
           (result == (UINT64_MAX / 10) && digit > (UINT64_MAX % 10))) {
            fprintf(stderr, "Error: Number too large\n");
            return 1;
        }

        result = result * 10 + digit;
        str++;
    }

    if (sievesOfEratosthenes(&primeArr, result) != 0) return 1;

    printPrimes(primeArr, result);
   /* for(i = 2; i<=result; i++) {
        if(!primeArr[i-1]) printf("%llu ", i);
    }; */
    printf("\n");

    free(primeArr);

}
