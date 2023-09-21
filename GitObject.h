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

#ifndef GIT_CONSTANT_H
#define GIT_CONSTANT_H
#include "GitConstant.h"
#endif

#ifndef INC_FILE_IO_H
#define INC_FILE_IO_H
#include "FileIO.h"
#endif

#ifndef INC_GIT_REPO_H
#define INC_GIT_REPO_H
#include "GitRepo.h"
#endif

#ifndef INC_GIT_ZLIB_H
#define INC_GIT_ZLIB_H
#include "GitZlib.h"
#endif

#ifndef INC_UNSOLVED_F_H
#define INC_UNSOLVED_F_H
#include "UnsolvedF.h"
#endif

#ifndef GIT_OBJECT_H
#define GIt_OBJECT_H

typedef struct GitObject
{
    unsigned parsed : 1;
    unsigned used : 1;
    unsigned int flags;
    unsigned char sha1[20];
    const char *type;
    struct object_list *refs;
    void *util;
    char *data;
    char *fmt;
} GitObj;

typedef struct ObjectList
{
    struct object *item;
    struct object_list *next;
    const char *name;
} ObjectList;

// Git Commit Obj
typedef struct GitCommit
{
    char *data;
} GitCommit;

// Git Tree Obj
typedef struct GitTree
{
    char *data;
} GitTree;

// Git Tag Obj
typedef struct GitTag
{
    char *data;
} GitTag;

// Git Blob Obj
typedef struct GitBlob
{
    char *data;
} GitBlob;

// 统一构造函数
void *GitObjConstructor(char *data, char *fmt)
{
    void *obj = malloc(sizeof(GitCommit));
    if (strcmp(fmt, "blob") == 0)
    {
        ((GitBlob *)obj)->data = data;
    }
    else if (strcmp(fmt, "tree") == 0)
    {
        ((GitTree *)obj)->data = data;
    }
    else if (strcmp(fmt, "commit") == 0)
    {
        ((GitCommit *)obj)->data = data;
    }
    else if (strcmp(fmt, "Tag") == 0)
    {
        ((GitTag *)obj)->data = data;
    }
    else
    {
        obj = NULL;
    }
    return obj;
}

// 统一序列化函数
char *Serialize(void *newObj, GitRep *repo, char *fmt)
{
    if (strcmp(fmt, "blob") == 0)
    {
        return ((GitBlob *)newObj)->data;
    }
    return NULL;
    //
}

// 统一解除序列化函数
void Deserialize(void *newObj, char *data, char *fmt)
{
    if (strcmp(fmt, "blob") == 0)
    {
        ((GitBlob *)newObj)->data = data;
    }
    //
}

void GitObjInit(GitObj *newObj, char *data)
{
    if (data != NULL)
        Deserialize(newObj, data, "obj");
    else
    {
        //
    }
}

// 读取对象，newRep为库，sha为sha值，fmt是读取获得的类别
void *ObjRead(GitRep *newRep, char *sha, char *fmt)
{
    short fileState;
    char *fileDirPath = StrConcat(StrConcat(newRep->gitdir, "\\objects\\"), SliceStr(sha, 0, 2));
    char *path = RepoFile(StrConcat(fileDirPath, "\\"), SliceStr(sha, 2, 255), 0, &fileState);

    PathProperty(path, &fileState);
    if (fileState != PATH_IS_FILE)
        return NULL;

    FILE *file;
    file = fopen(path, "rb");
    char *raw = NULL;
    uLong rawLength;
    char *content = ReadAllFile(file);

    UncompressStr(content, strlen(content), &raw, &rawLength);

    int x = (int)(strchr(raw, ' ') - raw);
    fmt = SliceStr(raw, 0, x);
    int y = (int)(strchr(raw, '\x00') - raw);

    if (strcmp(fmt, "commit") == 0 || strcmp(fmt, "tree") == 0 || strcmp(fmt, "tag") == 0 || strcmp(fmt, "blob") == 0)
    {
        return GitObjConstructor(SliceStr(raw, y + 1, 10086), fmt);
    }
    else
    {
        printf("Error Type!");
        return NULL;
    }
    // return fmt;
}

char *ObjWrite(void *newObj, GitRep *newRep, char *fmt)
{
    char *data = Serialize(newObj, newRep, fmt);
    char *result = StrConcat(StrConcat(StrConcat(fmt, " "), StrConcat(EncodeInt(strlen(data)), "\x00")), data);
    char *sha = (char *)ComputeHash((unsigned char *)result);

    if (newRep != NULL)
    {
        short fileState, pathState;
        char *fileDirPath = StrConcat(StrConcat(newRep->gitdir, "\\objects\\"), SliceStr(sha, 0, 2));
        char *path = RepoFile(StrConcat(fileDirPath, "\\"), SliceStr(sha, 2, 255), 1, &fileState);
        if (PathExist(path, &pathState))
        {
            char *compressResult;
            uLong crLength;
            FILE *file = fopen(path, "wb");
            CompressStr(result, &compressResult, &crLength);
            fprintf(file, "%s", compressResult);
            fclose(file);
        }
    }
    return sha;
}

// 寻找对象
char *ObjFind(GitRep *newRep, void *name, char *fmt, short follow)
{
    return (char *)name;
}

void CatFile(GitRep *newRep, void *newObj, char *fmt)
{
    char *obj = ObjRead(newRep, (char *)ObjFind(newRep, newObj, fmt, 1), fmt);
    printf("%s", Serialize(newObj, newRep, fmt));
}

typedef struct GitKeyValue
{
    char *key;
    char *value;
    struct GitKeyValue *next;
} GitKeyValue;

// 解析键值对文本数据并返回一个链表
GitKeyValue *KVLM_Parse(char *raw)
{
    GitKeyValue *head = (GitKeyValue *)malloc(sizeof(GitKeyValue));
    head->key = NULL;
    head->value = NULL;
    head->next = NULL;

    GitKeyValue *index = head;

    int start = 0, end = 0;
    while (1)
    {
        GitKeyValue *newKeyValue = (GitKeyValue *)malloc(sizeof(GitKeyValue));
        int space = strchr(raw, ' ') - raw;
        // int newLine = strchr(raw, '\n') - raw;
        if (strchr(raw, ' ') == NULL || strchr(raw, '\n') < strchr(raw, ' '))
        {
            index->next = newKeyValue;
            newKeyValue->key = "None";
            newKeyValue->value = SliceStr(raw, start + 1, 10086);
            return head;
        }
        space = strchr(raw, ' ') - raw;
        // newLine = strchr(raw, '\n') - raw;

        newKeyValue->key = SliceStr(raw, start, space);
        while (1)
        {
            // 找到值的末尾
            if (strchr(raw + end + 1, '\n') != NULL)
            {
                end = strchr(raw + end + 1, '\n') - raw;
                if (raw[end + 1] != ' ')
                    break;
            }
        }
        index->next = newKeyValue;
        newKeyValue->value = SliceStr(raw, space + 1, end - space - 1);
        start = end + 1;
        index = index->next;
    }

    return head;
}

#endif