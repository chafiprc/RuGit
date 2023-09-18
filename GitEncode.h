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

#ifndef GIT_ENCODE_H
#define GIT_ENCODE_H

#include <iconv.h>
int CommonEncode(const char *input, const char *from_encoding, const char *to_encoding, char **output)
{
    iconv_t cd;

    // 打开 iconv 转换描述符
    cd = iconv_open(to_encoding, from_encoding);
    if (cd == (iconv_t)-1)
    {
        perror("iconv_open");
        printf("Error Code %d\n", errno);
        return -1;
    }

    // 计算输入字符串和输出字符串的长度
    size_t in_len = strlen(input);
    size_t out_len = in_len * 4; // 预估输出字符串长度

    // 分配输出字符串的内存
    *output = (char *)malloc(out_len);
    if (*output == NULL)
    {
        perror("malloc");
        iconv_close(cd);
        return -1;
    }

    char *in_buf = (char *)input;
    char *out_buf = *output;

    // 执行字符编码转换
    size_t ret = iconv(cd, &in_buf, &in_len, &out_buf, &out_len);

    if (ret == (size_t)-1)
    {
        perror("iconv");
        free(*output);
        iconv_close(cd);
        return -1;
    }

    // 关闭 iconv 转换描述符
    iconv_close(cd);

    return 0;
}
#endif