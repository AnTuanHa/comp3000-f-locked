#include "cipher.h"
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
		b->b1[i] = (rand()%254+1);
		b->b2[i] = (rand()%254+1);
		b->b3[i] = (rand()%254+1);
		b->b4[i] = (rand()%254+1);
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
			unsigned char b1 = ((pt->b1[i] + (unsigned char)pwd[pwd_i])+255%255+1);
			ct->b1[i] = b1;
    }
    if(i>=8 && i<16){
			unsigned char b2 = ((pt->b2[i-8] + (unsigned char)pwd[pwd_i])+255%255+1);
			ct->b2[i-8] = b2;
    }
    if(i>=16 && i<24){
			unsigned char b3 = ((pt->b3[i-16] + (unsigned char)pwd[pwd_i])+255%255+1);
			ct->b3[i-16] = b3;
    }
    if(i>=24 && i<32){
			unsigned char b4 = ((pt->b4[i-24] + (unsigned char)pwd[pwd_i])+255%255+1);
			ct->b4[i-24] = b4;
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
			unsigned char b1 = ((ct->b1[i] - (unsigned char)pwd[pwd_i])+255%255-1%255);
			pt->b1[i] = b1;
    }
    if(i>=8 && i<16){
			unsigned char b2 = ((ct->b2[i-8] - (unsigned char)pwd[pwd_i])+255%255-1%255);
      pt->b2[i-8] = b2;
    }
    if(i>=16 && i<24){
			unsigned char b3 = ((ct->b3[i-16] - (unsigned char)pwd[pwd_i])+255%255-1%255);
      pt->b3[i-16] = b3;
    }
    if(i>=24 && i<32){
			unsigned char b4 = ((ct->b4[i-24] - (unsigned char)pwd[pwd_i])+255%255-1%255);
      pt->b4[i-24] = b4;
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
