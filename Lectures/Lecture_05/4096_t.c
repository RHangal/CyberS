/* 4096_t.c */

/*
gcc 4096_t.c --std=c89 -Wall -Wextra -Werror -Wpedantic
*/

#include "4096_t.h"

void bigshl(uint64_t *n, int shift) {
    if (shift == 0) return;

    int word_shift = shift / 64;
    int bit_shift = shift % 64;

    for (int i = S - 1; i >= word_shift; i--) {
        n[i] = (n[i - word_shift] << bit_shift);
        if (i - word_shift - 1 >= 0) {
            n[i] |= (n[i - word_shift - 1] >> (64 - bit_shift));
        }
    }
    for (int i = 0; i < word_shift; i++) {
        n[i] = 0;
    }
}

void bigshr(uint64_t *n, int shift) {
    if (shift == 0) return;

    int word_shift = shift / 64;
    int bit_shift = shift % 64;

    for (int i = 0; i < S - word_shift; i++) {
        n[i] = (n[i + word_shift] >> bit_shift);
        if (i + word_shift + 1 < S) {
            n[i] |= (n[i + word_shift + 1] << (64 - bit_shift));
        }
    }
    for (int i = S - word_shift; i < S; i++) {
        n[i] = 0;
    }
}



int bigcmp(uint64_t *a, uint64_t *b) {
    for (size_t i = S - 1; i < S; i--) {  
        if (a[i] > b[i]) return 1;  
        if (a[i] < b[i]) return -1;
    }
    return 0; 
}

uint64_t bigsub(uint64_t *min, uint64_t *sub, uint64_t *dif) {
      size_t i;
      uint64_t carry = 0, tmp;
      for (i = 0; i < S; i++) {
          tmp = min[i] - sub[i] - carry;
          carry = min[i] < sub[i];
          dif[i] = tmp;
    }
    return carry;
}

uint64_t bigadd(uint64_t *in0, uint64_t *in1, uint64_t *sum) {
    size_t i;
    uint64_t carry = 0, tmp;
    for(i = 0; i< S; i++) {
        tmp = in0[i] + in1[i] + carry;
        carry = (tmp < in0[i] || tmp < in1[i]);
        sum[i] = tmp;   
    }
    return carry;
}


uint64_t bigmul(uint64_t *in0, uint64_t *in1, uint64_t *out) {
       size_t i, j;
    uint64_t wrk[S * 2 + 1] = {0};  
    uint64_t temp[S * 2] = {0};  


    uint32_t *a = (uint32_t *) in0;
    uint32_t *b = (uint32_t *) in1;
    uint32_t *w = (uint32_t *) wrk;  
    uint32_t *t = (uint32_t *) temp;

    memset(wrk, 0, sizeof(wrk));  


    for (i = 0; i < S * 2; i++) { 
        memset(temp, 0, sizeof(temp)); 
        uint64_t carry = 0;

        for (j = 0; j < S * 2; j++) {  
            if (i + j >= S * 2) break; 

            uint64_t product = (uint64_t) a[i] * (uint64_t) b[j]; 
            uint64_t sum = (uint64_t) t[i + j] + product + carry; 

            t[i + j] = (uint32_t) sum; 
            carry = sum >> 32; 
        }
        t[i + j] += (uint32_t) carry;  

        bigadd(wrk, temp, wrk); 
    }

   
    for (i = 0; i < S; i++) {
        out[i] = ((uint64_t) w[2 * i + 1] << 32) | w[2 * i]; 
    }

     
    return 0;

}

/* Determine the log of a 4096-bit number in base 2^64 */
uint64_t biglog(uint64_t *in) {
    uint64_t log = S;
    while (log && !in[--log]) { }
    return log;
}

/* Determine the log of a 64-bit number in base 2 */
uint64_t lillog(uint64_t in) {
    uint64_t log = 0;
    while ((in >> log++) && log < 64) {}
    return log;
}

/* Find the address of the most significant bit of a 4096-bit integer */
uint64_t getmsb(uint64_t *n) {
    uint64_t b, l;
    b = biglog(n);
    l = lillog(n[b]);
    return b * 64 + l;
}


uint64_t bigdiv(uint64_t *num, uint64_t *den, uint64_t *quo, uint64_t *rem) {
    memset(quo, 0, S * sizeof(uint64_t)); 
    memset(rem, 0, S * sizeof(uint64_t)); 

    if (bigcmp(num, den) < 0) {
        memcpy(rem, num, S * sizeof(uint64_t)); 
        return 0;
    }

    uint64_t num_msb = getmsb(num); 
    uint64_t den_msb = getmsb(den); 

    int shift_amount = num_msb - den_msb;  
    uint64_t shifted_den[S] = {0};
    memcpy(shifted_den, den, S * sizeof(uint64_t));
    bigshl(shifted_den, shift_amount); 

    uint64_t temp_rem[S] = {0};
    memcpy(temp_rem, num, S * sizeof(uint64_t));

    for (int i = shift_amount; i >= 0; i--) {
        if (bigcmp(temp_rem, shifted_den) >= 0) {
            bigsub(temp_rem, shifted_den, temp_rem); 
            quo[i / 64] |= (1ULL << (i % 64));  
        }
        bigshr(shifted_den, 1); 
    }

    memcpy(rem, temp_rem, S * sizeof(uint64_t));  
    return 0;
}

uint64_t bigquo(uint64_t *num, uint64_t *den, uint64_t *quo) {
    uint64_t rem[S] = {0};  
    return bigdiv(num, den, quo, rem); 
}

uint64_t bigrem(uint64_t *num, uint64_t *den, uint64_t *rem) {
    uint64_t quo[S] = {0}; 
    return bigdiv(num, den, quo, rem); 
}

