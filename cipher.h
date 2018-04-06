#ifndef CIPHER_H
#define CIPHER_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 8

typedef struct{
  unsigned char b1[SIZE];
  unsigned char b2[SIZE];
  unsigned char b3[SIZE];
  unsigned char b4[SIZE];
} Blocks;

void gen_rdm(Blocks* b);
Blocks* encrypt(unsigned char* pwd, Blocks* pt, Blocks* ct);
Blocks* decrypt(unsigned char* pwd, Blocks* ct, Blocks* pt);
int is_valid(Blocks* plain, Blocks* plain_dec);
void blocks_to_string(Blocks* b, char* s);
Blocks* string_to_blocks(char* s);

#endif