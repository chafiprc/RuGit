#include "FileIO.h"

#define MAX_PATH_LENGTH 255

short gitState = 0;

struct GitRepository
{
    char *path;
    char *gitdir;
    char *conf;
};

typedef struct GitRepository GitRep;

GitRep newRep;

void init(char *path, char *gitdir, short force)
{
    newRep.path = path;
    newRep.gitdir = strcat(path, ".git");

    int gitdirState;
    PathProperty(gitdir, gitdirState);
    if (!(force || gitdirState == PATH_IS_DIR))
    {
        gitState = GIT_ERROR;
        return 0;
    }
}

void PrintErrorLog(char *w)
{
    printf("%s", w);
}

// 返回库文件下的路径
void RepoPath(GitRep r, char *path, char *returnPath)
{
    returnPath = strcat(r.gitdir, path);
}

void RepoDir(GitRep r, char *path, char *returnPath, short mkdir, short state)
{
    RepoPath(r, path, returnPath);
    short returnPathState;

    PathProperty(returnPath, returnPathState);
    if (returnPathState == PATH_IS_DIR)
        return;
    else
    {
        state = GIT_ERROR; // Not a directory
    }
    if (mkdir == 1)
    {
        char *command = "mkdir ";
        system(strcat(command, returnPath));
    }
    else
    {
        *returnPath = "";
    }
}

int main()
{

    int s;
    PathProperty("D:\\SoftLib\\Code\\Project\\RuGit\\.git\\config", &s);
    printf("%d", s);
}