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

#ifndef INC_FILE_IO_H
#define INC_FILE_IO_H
#include "FileIO.h"
#endif

#ifndef GIT_ZLIB_H
#define GIT_ZLIB_H
#include <zlib.h>

int CompressStr(const char *input, char **output, uLong *outputLength)
{
    uLong inputLength = strlen(input) + 1;                              // 包括结尾\0
    uLong outLength = compressBound(inputLength);                       // 预计压缩大小
    if ((*output = (char *)(malloc(sizeof(char) * outLength))) == NULL) // 申请空间
    {
        printf("No enough memory !");
        return -1;
    }

    if (compress((Bytef *)(*output), &outLength, (Bytef *)input, inputLength) != Z_OK)
    {
        printf("Compress Error!");
        return -1;
    }

    // printf("%s", *output);
    *outputLength = outLength;
    return 0;
}

// 解压缩 一定要保证inputlength的准确性，其中inputlength来源于压缩是输出的长度
int UncompressStr(const char *input, uLong inputLength, char **output, uLong *outputLength)
{
    uLong outLength = inputLength * 2;

    if ((*output = (char *)(malloc(sizeof(char) * outLength))) == NULL) // 申请空间
    {
        printf("No enough memory !");
        return -1;
    }
    int result = uncompress((Bytef *)(*output), &outLength, (const Bytef *)input, inputLength);

    do
    {
        if (result != Z_OK && result != Z_BUF_ERROR)
        {
            printf("Uncompress Error! Error Code:%d", result);
            return -1;
        }
        else if (result == Z_BUF_ERROR)
        {
            outLength *= 2;
            if ((*output = (char *)realloc(*output, sizeof(char) * (*outputLength))) == NULL) // 申请空间
            {
                printf("No enough memory !?");
                return -1;
            }
            result = uncompress((Bytef *)(*output), &outLength, (const Bytef *)input, inputLength);
        }
    } while (result == Z_BUF_ERROR);

    // printf("%s\n", *output);
    *outputLength = outLength;
    return 0;
}

#endif