#ifndef ATTR_H
#define ATTR_H

#include <sys/xattr.h>
#include <stdio.h>

#define KEY_SIZE 64

int attr_exists(const char *path, const char *name);
int setattr(const char *path, const char *name, const void *value, size_t size);
char* getattr(const char *path, const char *name);

#endif
