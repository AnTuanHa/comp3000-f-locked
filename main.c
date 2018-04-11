#include <stdio.h>
#include <unistd.h>

#include "attr.h"
#include "cipher.h"
#include "perm.h"

#define MAX_SIZE 1024

#define SECURITY_CIPHERTEXT "security.ciphertext"
#define SECURITY_PLAINTEXT "security.plaintext"
#define SECURITY_PERM "security.permissions"
#define KEY_CIPHERTEXT "ciphertext"
#define KEY_PLAINTEXT "plaintext"
#define KEY_PERM "permissions"

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
        // Ensure that the file the user is locking is owned by the user
        if (check_group(argv[1])) {
            if (lock_file(argv[1])) {
                printf("Failed to lock file '%s'\n", argv[1]);
                return -1;
            }
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
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_CIPHERTEXT, path);
        return -1;
    }
    if (setattr(path, KEY_PLAINTEXT, plain) < 0) {
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
        return -1;
    }

    char perms[7];
    sprintf(perms, "%d", get_perm(path));

    if (setattr(path, KEY_PERM, perms) < 0) {
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_PERM, path);
        return -1;
    }

    if (set_perm(path, 0000) < 0) {
        printf("Failed to change permissions on %s\n", path);
        return -1;
    }

    free(plain_block);
    free(cipher_block);
    return 0;
}

int unlock_file(const char *path)
{
    Blocks* plain_block;
    Blocks* cipher_block;
    Blocks* plain_dec_block = malloc(sizeof(Blocks));

    char cipher[MAX_SIZE];
    if (getxattr(path, SECURITY_CIPHERTEXT, cipher, MAX_SIZE - 1) < 0) {
        printf("Failed to get extended attribute '%s' on %s\n", SECURITY_CIPHERTEXT, path);
        return -1;
    }
    cipher_block = string_to_blocks(cipher);

    char plain[MAX_SIZE];
    if (getxattr(path, SECURITY_PLAINTEXT, plain, MAX_SIZE - 1) < 0) {
        printf("Failed to get extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
        return -1;
    }
    plain_block = string_to_blocks(plain);

    unsigned char pwd[SIZE * 4];
    printf("Enter your password to unlock: ");
    scanf("%s", pwd);

    decrypt(pwd, cipher_block, plain_dec_block);

    // Password is correct
    if (is_valid(plain_block, plain_dec_block) == 0) {
        printf("Unlocking file...\n");

        char perm[MAX_SIZE];
        if (getxattr(path, SECURITY_PERM, perm, MAX_SIZE - 1) < 0) {
            printf("Failed to get extended attribute '%s' on %s\n", SECURITY_PERM, path);
            return -1;
        }

        int p = (int)strtol(perm, (char **)NULL, 10);
        if (set_perm(path, p) < 0) {
            printf("Failed to change permissions on %s\n", path);
            return -1;
        }

        if (removexattr(path, SECURITY_CIPHERTEXT) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_CIPHERTEXT, path);
            return -1;
        }
        if (removexattr(path, SECURITY_PLAINTEXT) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
            return -1;
        }
        if (removexattr(path, SECURITY_PERM) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_PERM, path);
            return -1;
        }

        printf("Success\n");
    } else {
        printf("Wrong password\n");
    }

    free(plain_block);
    free(cipher_block);
    free(plain_dec_block);
    return 0;
}
