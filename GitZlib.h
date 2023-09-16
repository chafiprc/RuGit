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

// 压缩函数
int CompressString(const char *input, char **output, uLong *outputLength)
{
    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
    {
        printf("Compression initialization failed.\n");
        return -1;
    }

    uLong inputLength = strlen(input);

    *outputLength = compressBound(inputLength); // 估算输出缓冲区大小
    *output = (char *)malloc(sizeof(char) * (*outputLength));
    if (*output == NULL)
    {
        printf("Failed to allocate memory for compressed data.\n");
        deflateEnd(&stream); // 释放资源
        return -1;
    }

    stream.next_in = (Bytef *)input;
    stream.avail_in = inputLength;

    // 设置输出缓冲区
    stream.next_out = (Bytef *)(*output);
    stream.avail_out = *outputLength;

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END)
    {
        printf("Compression failed.\n");
        free(*output);       // 释放输出缓冲区内存
        deflateEnd(&stream); // 释放资源
        return -1;
    }

    // 更新输出长度
    *outputLength = stream.total_out;

    deflateEnd(&stream); // 释放资源
    return 0;
}

// 解压缩函数
int UncompressString(const char *input, char **output, uLong *outputLength)
{
    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    if (inflateInit(&stream) != Z_OK)
    {
        printf("Decompression initialization failed.\n");
        return -1;
    }

    uLong inputLength = strlen(input);

    *outputLength = 2 * inputLength; // 初始输出缓冲区大小为输入数据大小的两倍

    // 分配输出缓冲区
    *output = (char *)malloc(sizeof(char) * (*outputLength));
    if (*output == NULL)
    {
        printf("Failed to allocate memory for uncompressed data.\n");
        inflateEnd(&stream); // 释放资源
        return -1;
    }

    stream.next_in = (Bytef *)input;
    stream.avail_in = inputLength;

    int result;

    // 设置输出缓冲区
    stream.next_out = (Bytef *)(*output);
    stream.avail_out = *outputLength;

    do
    {
        // 执行解压缩
        result = inflate(&stream, Z_FINISH);

        // 检查解压缩结果
        if (result != Z_STREAM_END && result != Z_OK)
        {
            printf("Decompression failed.\n");
            free(*output);       // 释放输出缓冲区内存
            inflateEnd(&stream); // 释放资源
            return -1;
        }

        // 如果输出缓冲区不足，增加输出缓冲区大小并继续解压缩
        if (result == Z_BUF_ERROR)
        {
            *outputLength *= 2;
            *output = (char *)realloc(*output, sizeof(char) * (*outputLength));
            if (*output == NULL)
            {
                printf("Failed to reallocate memory for uncompressed data.\n");
                inflateEnd(&stream); // 释放资源
                return -1;
            }
            // 重新设置输出缓冲区
            stream.next_out = (Bytef *)(*output);
            stream.avail_out = *outputLength;
        }
    } while (result == Z_BUF_ERROR);

    // 更新输出长度
    *outputLength = stream.total_out;

    inflateEnd(&stream); // 释放资源
    return 0;
}

#endif