#ifndef COMMON_HEAD
#define COMMON_HEAD
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

#ifndef FILE_IO_H
#define FILE_IO_H

#define GIT_OK 0
#define GIT_ERROR 1

#ifndef FILE_STATE
#define FILE_STATE
#define PATH_IS_FILE 0
#define PATH_IS_DIR 1
#define PATH_ERROR 2

#endif

// 判断路径是文件，还是文件夹
void PathProperty(char *path, short *state)
{
    struct stat s;
    if (stat(path, &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            *state = PATH_IS_DIR;
        }
        else if (s.st_mode & S_IFREG)
        {
            *state = PATH_IS_FILE;
        }
        else
        {
            *state = PATH_ERROR;
        }
    }
    else
    {
        *state = PATH_ERROR;
    }
}

void CreateDir(char *path)
{
    char *command = "mkdir ";
    system(strcat(command, path));
}

// 读取配置文件
void ReadConfig(char *path, int *state)
{
    short configState;
    PathProperty(path, &configState);
    if (configState == PATH_IS_FILE) // 存在配置文件，读取配置文件
    {
    }
    else // 不存在配置文件，重新创建，并赋予默认值
    {
        char *command = "type nul>";
        system(strcat(command, path));
    }
}

void ReadDirContent(char *path)
{
    char *dirContent;
    DIR *dir = opendir(path);
    struct dirent *entry;
    int cnt = 0;
    while ((entry = readdir(dir)) != 0)
    {
        dirContent = entry->d_name;
        printf("%s\n", dirContent);
        cnt++;
    }
    // return dirContent;
}

#endif