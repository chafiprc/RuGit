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

// #include "GitHash.h" // UnDefined ERROR.  I HATE YOU!!!!!

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

/*

痛苦orz
这个头文件内的内容都是肝不动的代码
可以预见的是 在后续的更新中 这个头文件的内容会越来越多...
不会写的代码都用这样来假装写完了orz
void xxx{
    return; //TODO
}

*/