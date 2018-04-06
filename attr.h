#ifndef ATTR_H
#define ATTR_H

#include <sys/xattr.h>
#include <string.h>
#include <stdio.h>

#define KEY_SIZE 64

int setattr(const char *path, const char *name, const char *value);

#endif
