#ifndef CIPHER_H
#define CIPHER_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 8

typedef struct{
    unsigned int b1[SIZE];
    unsigned int b2[SIZE];
    unsigned int b3[SIZE];
    unsigned int b4[SIZE];
} Blocks;

void gen_rdm(Blocks* b);
void encrypt(char* pwd, Blocks* pt, Blocks* ct);
void decrypt(char* pwd, Blocks* ct, Blocks* pt);
int is_valid(Blocks* plain, Blocks* plain_dec);
void blocks_to_binary(Blocks* b, unsigned int* s);
Blocks* binary_to_blocks(unsigned int* s);

#endif
