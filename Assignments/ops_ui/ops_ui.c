#include "../4096_t/4096_t.h"
#include "ops_ui.h"
#include <string.h>


int bigmax(uint64_t* a, uint64_t* b) {
    return bigcmp(a,b) >= 0;
}

int mid_ui(uint64_t* big, uint64_t lil) {
    uint64_t lil_big[S] = {0};
    uint64_t rem[S] = {0};

    lil_big[0] = lil;

    bigrem(big, lil_big, rem);

    for(size_t i = 0; i < S; i++) {
        if(rem[i] != 0) return 0;
    }
    return 1;
}

uint64_t add_ui(uint64_t *big, uint64_t lil, uint64_t *out){
    uint64_t lil_big[S] = {0};
   
    lil_big[0] = lil;

    return bigadd(big, lil_big, out);
}

uint64_t sub_ui(uint64_t *big, uint64_t lil, uint64_t *out){
    uint64_t lil_big[S] = {0};
   
    lil_big[0] = lil;

    if(!bigmax(big, lil_big)) {
        memset(out, 0, S * sizeof(uint64_t));          
        return 1;
    }

    return bigsub(big, lil_big, out);
}

uint64_t mul_ui(uint64_t *big, uint64_t lil, uint64_t *out){
    uint64_t lil_big[S] = {0};
   
    lil_big[0] = lil;

    return bigmul(big, lil_big, out);
}

uint64_t quo_ui(uint64_t *big, uint64_t lil, uint64_t *out){
    uint64_t lil_big[S] = {0};
   
    lil_big[0] = lil;

    return bigquo(big, lil_big, out);
}

uint64_t rem_ui(uint64_t *big, uint64_t lil, uint64_t *out){
    uint64_t lil_big[S] = {0};
    
    lil_big[0] = lil;

    return bigrem(big, lil_big, out);

}

