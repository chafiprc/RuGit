#include "FileIO.h"

short gitState = 0;

struct GitRepository
{
    char *worktree;
    char *gitdir;
    char *conf;
};

char *empty = "";

typedef struct GitRepository GitRep;

void IsGitError()
{
    if (gitState == GIT_ERROR)
    {
        printf("RUNNING ERROR! Program exit.");
        exit(0);
    }
}

void init(GitRep *newRep, char *path, short force)
{
    newRep->worktree = path;
    newRep->gitdir = StrConcat(path, "\\.git");

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
// r是库类型，path是路径，returnPath是返回的路径
void RepoPath(GitRep r, char *path, char *returnPath)
{
    strcpy(returnPath, r.gitdir);
    strcat(returnPath, path);
}

// 返回库文件下的路径（文件夹），可以决定是否创建
// r是库类型，path是路径，returnPath是返回的路径， mkdir是1时若路径不存在则创建，否则不创建；state是状态码
void RepoDir(GitRep r, char *path, char *returnPath, short mkdir, short *state)
{
    *state = GIT_OK;
    RepoPath(r, path, returnPath);
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
        returnPath = empty;
    }
}

// 返回库文件下的路径（文件），可以决定是否创建
// path是路径，filename是文件名， mkdir是1时若路径不存在则创建，否则不创建；state是状态码
char *RepoFile(char *path, char *filename, short mkdir, short *state)
{
    short dirState = 0;
    PathProperty(path, &dirState);
    if (dirState == PATH_IS_DIR)
    {
        return StrConcat(path, filename);
    }
    else if (mkdir)
    {
        CreateDir(path);
        return StrConcat(path, filename);
    }
    else
    {
        gitState = GIT_ERROR;
        return NULL;
    }
}

// 创建本地库
GitRep *RepoCreate(char *path)
{
    GitRep *newRep = (GitRep *)malloc(sizeof(GitRep));
    init(newRep, path, 1);

    short repState = 0;
    PathProperty(newRep->worktree, &repState);
    if (PathExist(path, &repState) == PATH_IS_EXIST)
    {
        if (!(repState == PATH_IS_DIR))
        {
            gitState = GIT_ERROR;
            return NULL;
        }
        else if (!DirEmpty(newRep->gitdir))
        {
            gitState = GIT_ERROR;
            return NULL;
        }
    }
    else
    {
        CreateDir(newRep->worktree);
    }

    char *tempReturnPath = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    short mkdirState = 0;
    RepoDir(*newRep, "\\branches", tempReturnPath, 1, &mkdirState);
    RepoDir(*newRep, "\\objects", tempReturnPath, 1, &mkdirState);
    RepoDir(*newRep, "\\refs\\tags", tempReturnPath, 1, &mkdirState);
    RepoDir(*newRep, "\\refs\\heads", tempReturnPath, 1, &mkdirState);

    FILE *file;
    char *filePath;
    short fileState = 0;

    // .git/description
    filePath = RepoFile(newRep->gitdir, "\\description", 1, &fileState);
    file = fopen(filePath, "w");
    fprintf(file, "Unnamed repository; edit this file 'description' to name the repository.\n");
    fclose(file);

    //.git/head
    filePath = RepoFile(newRep->gitdir, "\\HEAD", 1, &fileState);
    file = fopen(filePath, "w");
    fprintf(file, "ref: refs/heads/master\n");
    fclose(file);

    //.git/config
    filePath = RepoFile(newRep->gitdir, "\\config", 1, &fileState);
    file = fopen(filePath, "w");
    WriteDefaultConfig(file);
    fclose(file);

    return &newRep;
}

int main()
{

    int s;
    // PathProperty("D:\\SoftLib\\Code\\Project\\RuGit\\.git\\config", &s);
    // ReadDirContent("D:\\SoftLib");
    RepoCreate("C:\\Users\\chafiprc\\Desktop\\newGitRepo");
    printf("%d", gitState);
    // printf("%d", s);
}