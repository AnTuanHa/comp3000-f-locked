#include "attr.h"

int setattr(const char *path, const char *name, const char *value)
{
    int len = strlen(value);
    char key[KEY_SIZE];
    sprintf(key, "security.%s", name);
    return setxattr(path, key, value, len, 0);
}
