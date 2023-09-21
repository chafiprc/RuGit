#ifndef COMMON_HEAD
#define COMMON_HEAD
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <sys/stat.h>
#include <dirent.h>
#include <direct.h>
#endif

#include "GitRepo.h"

#include "GitEncode.h"

// #include "GitHash.h"

#ifndef UNSOLVED_F_H
#define UNSOLVED_F_H
unsigned char *ComputeHash(unsigned char *data)
{
    // Waiting for further work.
    return NULL;
} // Finish work in GitHash.h .But include error!

#define Encode(x, y) CommonEncode(x, "ASCII", "UTF-8", y)

#define Decode(x, y) CommonEncode(x, "UTF-8", "ASCII", y)

// char *Encode(char *data)
// {
//     // Waiting for further work.
//     return NULL;
// } ReDefine.

char *EncodeInt(int data)
{
    // Waiting for further work.
    return NULL;
}

// char *Decode(char *data)
// {
//     // Waiting for further work.
//     return NULL;
// }
#endif