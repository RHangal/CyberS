#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../list_t/list_t.h"
#include "../transaction_t/transaction.h"

/**
 * We need a blockchain struct and a block struct
 * We need to define functions:
 * Create Blockchain
 * Create Block
 * Create Genesis Block(Maybe)
 * Free Blockchain(Evil Function)
 * Mine Block
 *
 *
 **/

typedef struct block {
    uint64_t index;
    uint64_t timestamp;
    uint32_t hash[8];
    uint32_t merkleroot[8];
    uint64_t nonce;

    list_t transactions;

    struct block* next;
} block_t;

typedef struct blockchain {
    block_t* head;
    block_t* tail;

    uint64_t size;
} blockchain_t;


blockchain_t* blockchain_new();

void blockchain_add(blockchain_t* bc,block_t* new_block);

void free_blockchain(blockchain_t* bc); /*No idea why im making this other than evil reasons*/

#endif
