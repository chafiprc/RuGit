#include "openssl\sha.h"

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

#ifndef GIT_HASH_H
#define GIT_HASH_H

unsigned char *ComputeHash(unsigned char *data)
{
    unsigned char *sha = (unsigned char *)(malloc(sizeof(unsigned char) * SHA_DIGEST_LENGTH));
    SHA1(data, sizeof(data) - 1, sha);
    return sha;
}

#endif