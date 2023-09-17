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

struct GitObject
{
    char *data;
    char *fmt;
};

typedef struct GitObject GitObj;

// Git Commit Obj
typedef struct GitCommit
{
    char *data;
} GitCommit;

GitCommit *GitCommitConstruct(char *data)
{
}

// Git Tree Obj
typedef struct GitTree
{
    char *data;
} GitTree;

GitTree *GitTreeConstruct(char *data)
{
}

// Git Tag Obj
typedef struct GitTag
{
    char *data;
} GitTag;

GitTag *GitTagConstruct(char *data)
{
}

// Git Blob Obj
typedef struct GitBlob
{
    char *data;
} GitBlob;

GitBlob *GitBlobConstruct(char *data)
{
}

char *Serialize(GitObj *newObj, GitRep *repo)
{
    return NULL;
    //
}

void Deserialize(GitObj *newObj, char *data)
{
    //
}

void GitObjInit(GitObj *newObj, char *data)
{
    if (data != NULL)
        Deserialize(newObj, data);
    else
    {
        //
    }
}

char *ObjRead(GitRep *newRep, char *sha)
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
    UncompressString(ReadAllFile(file), &raw, &rawLength);

    int x = (int)(strstr(raw, " ") - raw);
    char *fmt = SliceStr(raw, 0, x);
    // int y = (int)(strstr(raw + x, "\x00") - raw - x);

    return fmt;
}

char *ObjWrite(GitObj *newObj, GitRep *newRep)
{
    char *data = Serialize(newObj, newRep);
    char *result = StrConcat(StrConcat(StrConcat(newObj->fmt, " "), StrConcat(EncodeInt(strlen(data)), "\x00")), data);
    char *sha = ComputeHash(result);

    if (newRep != NULL)
    {
        short fileState, pathState;
        char *fileDirPath = StrConcat(StrConcat(newRep->gitdir, "\\objects\\"), SliceStr(sha, 0, 2));
        char *path = RepoFile(StrConcat(fileDirPath, "\\"), SliceStr(sha, 2, 255), 1, &fileState);
        if (PathExist(path, &pathState))
        {
            FILE *file = fopen(path, "wb");
            fprintf(file, "%s", result);
            fclose(file);
        }
    }
    return sha;
}

GitObj *ObjFind(GitRep *newRep, GitObj *name, char *fmt, short follow)
{
    return name;
}

void CatFile(GitRep *newRep, GitObj *newObj, char *fmt)
{
    // char *obj = ObjRead(newRep, (char *)ObjFind(newRep, newObj, fmt, 1));
    printf("%s", Serialize(newObj, newRep));
}

struct KeyValue
{
    char *key;
    char *value;
    struct KeyValue *next;
};

// 解析键值对文本数据并返回一个链表
struct KeyValue *KVLM_Parse(const char *raw)
{
    struct KeyValue *head = NULL;
    struct KeyValue *current = NULL;
    const char *line = raw;

    while (*line != '\0')
    {
        const char *space = strchr(line, ' ');
        const char *newline = strchr(line, '\n');

        if (newline == NULL || (space != NULL && space < newline))
        {
            // 当前行是一个键值对
            size_t key_len = space - line;
            char *key = strndup(line, key_len);

            // 找到值的结束位置
            const char *value_start = space + 1;
            const char *value_end = newline;
            while (*value_start == ' ')
            {
                value_start++;
            }

            size_t value_len = value_end - value_start;
            char *value = strndup(value_start, value_len);

            // 创建键值对节点
            struct KeyValue *kv = (struct KeyValue *)malloc(sizeof(struct KeyValue));
            kv->key = key;
            kv->value = value;
            kv->next = NULL;

            // 将节点添加到链表中
            if (head == NULL)
            {
                head = kv;
                current = kv;
            }
            else
            {
                current->next = kv;
                current = kv;
            }

            line = newline + 1;
        }
        else
        {
            // 当前行是消息内容
            char *message = strdup(line + 1);

            // 创建键值对节点，键为NULL
            struct KeyValue *kv = (struct KeyValue *)malloc(sizeof(struct KeyValue));
            kv->key = NULL;
            kv->value = message;
            kv->next = NULL;

            // 将节点添加到链表中
            if (head == NULL)
            {
                head = kv;
                current = kv;
            }
            else
            {
                current->next = kv;
                current = kv;
            }

            break;
        }
    }
    return head;
}

#endif