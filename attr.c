#include "attr.h"

int attr_exists(const char *path, const char *name)
{
    return (getxattr(path, name, NULL, 0) != -1);
}

int setattr(const char *path, const char *name, const char *value)
{
    int len = strlen(value);
    char key[KEY_SIZE];
    sprintf(key, "security.%s", name);
    return setxattr(path, key, value, len, 0);
}
