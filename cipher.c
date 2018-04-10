#include "cipher.h"
#include <stdio.h>
/*****
DISCLAIMER

DO NOT USE A PASSWORD CONSTAINING ONLY "0"
PASSWORD SCHEME WILL ONLOCK ON ANY NUMBER OF "0" IF THAT IS THE PASSWORD

ex:
  password : "000"
    "0"     -> can unlock
    "00"    -> can unlock
    "000"   -> can unlock
    .....
*****/
void gen_rdm(Blocks* b){
	srand((unsigned int) time(NULL));
	size_t i;
	for(i=0; i<SIZE; i++){
		b->b1[i] = rand();
		b->b2[i] = rand();
		b->b3[i] = rand();
		b->b4[i] = rand();
	}
}

void encrypt(char * pwd, Blocks* pt, Blocks* ct){
  size_t i;
  size_t pwd_i = 0;
  for(i=0; i<SIZE*4; i++){
    if(pwd[pwd_i] == '\0'){
      pwd_i = 0;
    }
    if(i<8){
      ct->b1[i] = (unsigned char)(pt->b1[i] + (unsigned char)pwd[pwd_i]);
    }
    if(i>=8 && i<16){
      ct->b2[i-8] = (unsigned char)(pt->b2[i-8] + (unsigned char)pwd[pwd_i]);
    }
    if(i>=16 && i<24){
      ct->b3[i-16] = (unsigned char)(pt->b3[i-16] + (unsigned char)pwd[pwd_i]);
    }
    if(i>=24 && i<32){
      ct->b4[i-24] = (unsigned char)(pt->b4[i-24] + (unsigned char)pwd[pwd_i]);
    }
    pwd_i++;
  }
}

void decrypt(char* pwd, Blocks* ct, Blocks* pt){
  size_t i;
  size_t pwd_i = 0;
  for(i=0; i<SIZE*4; i++){
    if(pwd[pwd_i] == '\0'){
      pwd_i = 0;
    }
    if(i<8){
      pt->b1[i] = (unsigned char)(ct->b1[i] - (unsigned char)pwd[pwd_i]);
    }
    if(i>=8 && i<16){
      pt->b2[i-8] = (unsigned char)(ct->b2[i-8] - (unsigned char)pwd[pwd_i]);
    }
    if(i>=16 && i<24){
      pt->b3[i-16] = (unsigned char)(ct->b3[i-16] - (unsigned char)pwd[pwd_i]);
    }
    if(i>=24 && i<32){
      pt->b4[i-24] = (unsigned char)(ct->b4[i-24] - (unsigned char)pwd[pwd_i]);
    }
    pwd_i++;
  }
}
//0 -> true
//-1 -> false
int is_valid(Blocks* plain, Blocks* plain_dec){
  size_t i;
  int isvalid = 0;
  for(i=0; i<SIZE; i++){
    if(
    (plain->b1[i] != plain_dec->b1[i])||
    (plain->b2[i] != plain_dec->b2[i])||
    (plain->b3[i] != plain_dec->b3[i])||
    (plain->b4[i] != plain_dec->b4[i])
  ){
      isvalid = -1;
      break;
    }
  }
  return isvalid;
}

void blocks_to_string(Blocks* b, char* s){
  for(int i=0;i<SIZE; i++){
    s[i] = (char)b->b1[i];
    s[i+(SIZE)] = (char)b->b2[i];
    s[i+(2*SIZE)] = (char)b->b3[i];
    s[i+(3*SIZE)] = (char)b->b4[i];
  }
}
//allocates Blocks* in HEAP
/****************************/
//make sure to free when done
/****************************/
Blocks* string_to_blocks(char* s){
  Blocks* b = malloc(sizeof(Blocks));
  for(int i=0; i<SIZE; i++){
    b->b1[i] = (unsigned char)s[i];
    b->b2[i] = (unsigned char)s[i+(SIZE)];
    b->b3[i] = (unsigned char)s[i+(2*SIZE)];
    b->b4[i] = (unsigned char)s[i+(3*SIZE)];
  }
  return b;
}
