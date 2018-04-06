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

char* getattr(const char* path, const char* name)
{
    int i;
    void* yourKey;
    char key[KEY_SIZE];

    sprintf(key, "security.%s", name);

    //Getting the key from the file
    i = getxattr(path, key, NULL, 0);

    //If there is no key
    if (i == -1)
      return NULL;

    getxattr(path, key, yourKey, i);

    char* charKey = yourKey;
    return charKey;
}
