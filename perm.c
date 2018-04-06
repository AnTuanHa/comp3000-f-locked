#include "perm.h"

int check_group(const char* path) 
{
    struct stat file;          
    stat(path, &file);  //Getting the file that the path points to

    //Checks if root is the owner of the file
    if (file.st_uid == 0)
        return 0;

    //Checks if the user's id is the same as the file owner's
    if (file.st_uid != getuid())
        return 0;
    return 1;
}

int get_perm(const char* path) 
{
    struct stat file;  
    stat(path, &file);

    return file.st_mode;
}
