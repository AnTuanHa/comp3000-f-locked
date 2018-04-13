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

#define ESTDERR -1
#define EWRONG_PASSWORD -2
#define EEMPTY_PASSWORD -3
#define EFILE_NOT_EXISTS -4
#define EFILE_NOT_OWNED_BY_USER -5
#define EFILE_CHANGE_PERMISSIONS -6
#define EFILE_SET_XATTR -7
#define EFILE_GET_XATTR -8
#define EFILE_RM_XATTR -9

int lock_file(const char *path);
int unlock_file(const char *path);

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        printf("usage: %s PATH-TO-FILE\n", argv[0]);
        return ESTDERR;
    }

    if (access(argv[1], F_OK) < 0) {
        printf("File '%s' does not exist\n", argv[1]);
        return EFILE_NOT_EXISTS;
    }

    // Ensure that the file the user is locking is owned by the user
    if (!check_group(argv[1])) {
        printf("File '%s' is not owned by current user!\n", argv[1]);
        printf("Aborting file locking\n");
        return EFILE_NOT_OWNED_BY_USER;
    }

    int ret = 0;

    // Lock does not exist
    if (!attr_exists(argv[1], SECURITY_CIPHERTEXT) &&
            !attr_exists(argv[1], SECURITY_PLAINTEXT)) {
        ret = lock_file(argv[1]);
        if (ret)
            printf("Failed to lock file '%s'\n", argv[1]);
    } else {
        ret = unlock_file(argv[1]);
        if (ret)
            printf("Failed to unlock file '%s'\n", argv[1]);
    }

    return ret;
}

int lock_file(const char *path)
{
    Blocks *plain_block = malloc(sizeof(Blocks));
    Blocks *cipher_block = malloc(sizeof(Blocks));
    char pwd[SIZE * 4];
    printf("Enter your password: ");
    fgets(pwd, sizeof(pwd), stdin);
    gen_rdm(plain_block);

    if (pwd[0] == '\n' || pwd[0] == '\0') {
        printf("Empty password given, aborting file locking\n", path);
        return EEMPTY_PASSWORD;
    }

    encrypt(pwd, plain_block, cipher_block);
    char plain[SIZE * 4];
    char cipher[SIZE * 4];
    blocks_to_string(plain_block, plain);
    blocks_to_string(cipher_block, cipher);

    char perms[7];
    sprintf(perms, "%d", get_perm(path));

    if (set_perm(path, 0000) < 0) {
        printf("Failed to change permissions on %s\n", path);
        return EFILE_CHANGE_PERMISSIONS;
    }

    if (setattr(path, KEY_CIPHERTEXT, cipher) < 0) {
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_CIPHERTEXT, path);
        return EFILE_SET_XATTR;
    }
    if (setattr(path, KEY_PLAINTEXT, plain) < 0) {
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
        return EFILE_SET_XATTR;
    }
    if (setattr(path, KEY_PERM, perms) < 0) {
        printf("Failed to set extended attribute '%s' on %s\n", SECURITY_PERM, path);
        return EFILE_SET_XATTR;
    }

    printf("File successfully locked with the given password\n");

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
        return EFILE_GET_XATTR;
    }
    cipher_block = string_to_blocks(cipher);

    char plain[MAX_SIZE];
    if (getxattr(path, SECURITY_PLAINTEXT, plain, MAX_SIZE - 1) < 0) {
        printf("Failed to get extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
        return EFILE_GET_XATTR;
    }
    plain_block = string_to_blocks(plain);

    char pwd[SIZE * 4];
    printf("Enter your password to unlock: ");
    fgets(pwd, sizeof(pwd), stdin);

    decrypt(pwd, cipher_block, plain_dec_block);

    // Password is correct
    if (is_valid(plain_block, plain_dec_block) == 0) {
        printf("Unlocking file...\n");

        char perm[MAX_SIZE];
        if (getxattr(path, SECURITY_PERM, perm, MAX_SIZE - 1) < 0) {
            printf("Failed to get extended attribute '%s' on %s\n", SECURITY_PERM, path);
            return EFILE_GET_XATTR;
        }
        int p = (int)strtol(perm, (char **)NULL, 10);

        if (removexattr(path, SECURITY_CIPHERTEXT) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_CIPHERTEXT, path);
            return EFILE_RM_XATTR;
        }
        if (removexattr(path, SECURITY_PLAINTEXT) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_PLAINTEXT, path);
            return EFILE_RM_XATTR;
        }
        if (removexattr(path, SECURITY_PERM) < 0) {
            printf("Failed to remove extended attribute '%s' on %s\n", SECURITY_PERM, path);
            return EFILE_RM_XATTR;
        }

        if (set_perm(path, p) < 0) {
            printf("Failed to change permissions on %s\n", path);
            return EFILE_CHANGE_PERMISSIONS;
        }

        printf("Success\n");
    } else {
        printf("Wrong password\n");
        return EWRONG_PASSWORD;
    }

    free(plain_block);
    free(cipher_block);
    free(plain_dec_block);
    return 0;
}
