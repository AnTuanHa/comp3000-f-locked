#ifndef PERM_H
#define PERM_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>

#define KEY_SIZE 64

int check_group(const char *path);
int get_perm(const char *path);
int set_perm(const char *path, int value);

#endif

