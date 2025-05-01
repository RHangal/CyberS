#include "blockchain.h"

#include <stdio.h>
#include <stint.h>
#include <stlib.h>
#include <string.h>
#include <time.h>


blockchain_t* blockchain_create() {
    blockchain_t* bc = malloc(sizeof(blockchain_t));
    if(!bc) {
        fprinf(stderr,"Memory Allocation Failed, Exiting\n");
        exit(1);
    }

    bc->head = NULL;
    bc->tail = NULL;
    bc->size = 0;

    return bc
}







