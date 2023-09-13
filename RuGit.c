#include "FileIO.h"

#define MAX_PATH_LENGTH 255

short gitState = 0;

struct GitRepository
{
    char *worktree;
    char *gitdir;
    char *conf;
};

typedef struct GitRepository GitRep;

void init(GitRep *newRep, char *path, short force)
{
    newRep->worktree = path;
    newRep->gitdir = strcat(path, ".git");

    short gitdirState;
    PathProperty((*newRep).gitdir, &gitdirState);
    if (!(force || gitdirState == PATH_IS_DIR))
    {
        gitState = GIT_ERROR;
        return;
    }
}

void PrintErrorLog(char *w)
{
    printf("%s", w);
}

// 返回库文件下的路径
// r是库类型，path是路径数组，parameterNum是参数个数，returnPath是返回的路径
void RepoPath(GitRep r, char *path[], int paramaterNum, char *returnPath)
{
    strcpy(returnPath, r.gitdir);
    for (int i = 0; i < paramaterNum; i++)
        returnPath = strcat(returnPath, path[i]);
}

// 返回库文件下的路径（文件夹），可以决定是否创建
// r是库类型，path是路径数组，parameterNum是参数个数，returnPath是返回的路径， mkdir是1时若路径不存在则创建，否则不创建；state是状态码
void RepoDir(GitRep r, char *path[], int paramaterNum, char *returnPath, short mkdir, short *state)
{
    *state = GIT_OK;
    RepoPath(r, path, paramaterNum, returnPath);
    short returnPathState = 0;

    PathProperty(returnPath, &returnPathState);
    if (returnPathState == PATH_IS_DIR)
        return;
    else
    {
        *state = GIT_ERROR; // Not a directory
    }
    if (mkdir == 1)
    {
        CreateDir(returnPath);
    }
    else
    {
        *returnPath = "";
    }
}

// 返回库文件下的路径（文件），可以决定是否创建
// r是库类型，path是路径数组，parameterNum是参数个数，returnPath是返回的路径， mkdir是1时若路径不存在则创建，否则不创建；state是状态码
void RepoFile(GitRep r, char *path[], int paramaterNum, char *returnPath, short mkdir, short *state)
{
    RepoDir(r, path, paramaterNum - 1, returnPath, mkdir, state); // 是文件，因此最后一个参数是文件名
    if (state == GIT_OK)
    {
        returnPath = strcat(returnPath, path[paramaterNum - 1]);
        return;
    }
}

// 创建本地库
void RepoCreate(char *path)
{
    GitRep newRep;
    init(&newRep, path, 1);

    short repState = 0;
    PathProperty(newRep.worktree, &repState);
    if (!(repState == PATH_IS_DIR))
    {
        gitState = GIT_ERROR;
        return;
    }
    // if(empty)
}

int main()
{

    int s;
    // PathProperty("D:\\SoftLib\\Code\\Project\\RuGit\\.git\\config", &s);
    ReadDirContent("D:\\SoftLib");
    // printf("%d", s);
}