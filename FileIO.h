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

#define MAX_PATH_LENGTH 255

#define GIT_OK 0
#define GIT_ERROR 1

#ifndef FILE_STATE
#define FILE_STATE
#define PATH_IS_FILE 0
#define PATH_IS_DIR 1
#define PATH_ERROR 2

#define PATH_IS_NOT_EXIST 0
#define PATH_IS_EXIST 1

#endif

// 拼接字符串，额外申请可见，返回一个新的字符串
char *StrConcat(char *s1, char *s2)
{
    char *newStr = (char *)(malloc(sizeof(char) * MAX_PATH_LENGTH));
    strcpy(newStr, s1);
    strcat(newStr, s2);
    return newStr;
}

// 判断路径是否存在
short PathExist(char *path, short *state)
{
    if (!access(path, F_OK))
        return PATH_IS_EXIST;
    else
        return PATH_IS_NOT_EXIST;
}

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
    system(StrConcat(command, path));
}

void CreateGitFile(char *path)
{
    char *command = "type nul>";
    system(StrConcat(command, path));
}

void WriteDefaultConfig(FILE *file)
{
    fprintf(file, "core.repositoryformatversion=0\n");
    fprintf(file, "core.filemode=false\n");
    fprintf(file, "core.bare=false\n");
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
        CreateGitFile(path);
    }
}

// 判断文件夹是否为空，空则返回1
short DirEmpty(char *path)
{
    short dirState = 0;
    PathProperty(path, &dirState);
    if (dirState != PATH_IS_DIR)
    {
        return 0; // 路径文件夹不存在
    }
    DIR *dir = opendir(path);
    struct dirent *entry;
    if ((entry = readdir(dir)) != 0)
    {
        return 0; // 路径文件夹非空
    }
    else
    {
        return 1; // 路径文件夹为空
    }
}

void ReadDirContent(char *path, char *state)
{
    short dirState = 0;
    PathProperty(path, &dirState);
    if (dirState != PATH_IS_DIR)
    {
        *state = GIT_ERROR;
        return;
    }
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