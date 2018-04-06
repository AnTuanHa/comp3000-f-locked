#include <stdio.h>
#include <unistd.h>

#include "attr.h"
#include "cipher.h"

#define SECURITY_CIPHERTEXT "security.ciphertext"
#define SECURITY_PLAINTEXT "security.plaintext"
#define KEY_CIPHERTEXT "ciphertext"
#define KEY_PLAINTEXT "plaintext"

int lock_file(const char *path);
int unlock_file(const char *path);

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("usage: %s PATH-TO-FILE\n", argv[0]);
        return -1;
    }

    if (access(argv[1], F_OK) < 0) {
        printf("File '%s' does not exist\n", argv[1]);
        return -1;
    }

    // Lock does not exist
    if (!attr_exists(argv[1], SECURITY_CIPHERTEXT) &&
            !attr_exists(argv[1], SECURITY_PLAINTEXT)) {
        if (lock_file(argv[1])) {
            printf("Failed to lock file '%s'\n", argv[1]);
            return -1;
        }
    } else {
        if (unlock_file(argv[1])) {
            printf("Failed to unlock file '%s'\n", argv[1]);
            return -1;
        }
    }

    return 0;
}

int lock_file(const char *path)
{
    Blocks *plain_block = malloc(sizeof(Blocks));
    Blocks *cipher_block = malloc(sizeof(Blocks));
    unsigned char pwd[SIZE * 4];
    printf("Enter your password: ");
    scanf("%s", pwd);
    gen_rdm(plain_block);

    encrypt(pwd, plain_block, cipher_block);
    char plain[SIZE * 4];
    char cipher[SIZE * 4];
    blocks_to_string(plain_block, plain);
    blocks_to_string(cipher_block, cipher);

    if (setattr(path, KEY_CIPHERTEXT, cipher) < 0) {
        printf("Failed to set extended attribute 'security.%s' on %s\n", KEY_CIPHERTEXT, path);
        return -1;
    }
    if (setattr(path, KEY_PLAINTEXT, plain) < 0) {
        printf("Failed to set extended attribute 'security.%s' on %s\n", KEY_PLAINTEXT, path);
        return -1;
    }

    free(plain_block);
    free(cipher_block);
    return 0;
}

int unlock_file(const char *path)
{
    //TODO: Implement unlock
    return 0;
}
