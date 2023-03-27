#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include "tools.h"
void getCurDllPath(char *path)
{
    Dl_info info;
    dladdr((void *)getCurDllPath, &info);

    strcpy(path, info.dli_fname);
    int i, pos;
    for (i = 0; path[i] != '\0'; i++)
        if (path[i] == '/')
            pos = i;
    path[pos + 1] = '\0';
    return;
}