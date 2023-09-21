#include "FileIO.h"
#include "GitObject.h"
#include "GitRepo.h"
#include "GitZlib.h"
// #include "GitHash.h"
#include "UnsolvedF.h"

char *ObjectHash(FILE *file, char *fmt, GitRep *newRep)
{
    void *obj;
    char *data = ReadAllFile(file);
    obj = GitObjConstructor(data, fmt);
    return ObjWrite(obj, newRep, fmt);
}

void CMD_HashObject(short isWrite, char *fmt, char *path)
{
    GitRep *newRep = (GitRep *)malloc(sizeof(GitRep));
    if (isWrite)
    {
        newRep = RepoFind(NULL);
    }
    else
    {
        newRep = NULL;
    }

    FILE *file = fopen(path, "rb");
    char *sha = ObjectHash(file, fmt, newRep);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("RuGit. version a0.37-beta(Early development version.)\nCreated by Chafiprc.\nUse '--help' for more options.");
    }
    else
    {
        if (strcmp(argv[1], "init") == 0)
        {
            char *initPath;
            if (argc == 2)
            {
                // printf("%s\n", GetCurrentWorkPath());
                RepoCreate(GetCurrentWorkPath());
                printf("%d", gitState);
            }
            else if (argc >= 3 && strcmp(argv[2], "help") == 0)
            {
                printf("Where to create the repository.\nUsage:\ninit [name(option)]     Initialise a RuGit Repo. If this parameter is filled in, a folder named with 'name' will be generated in the current directory, and the initialisation will be done in that folder, otherwise the initialisation will be done directly in the current directory.\n\n");
            }
            else
            {
                // printf("%s", StrConcat(GetCurrentWorkPath(), initPath));
                initPath = StrConcat("\\", argv[2]);
                RepoCreate(StrConcat(GetCurrentWorkPath(), initPath));
                printf("%d", gitState);
            }
        }
        else if (strcmp(argv[1], "cat-file") == 0)
        {
            printf("ATTENTION!!! This command might be not work well.\n");
            if (argc == 2)
            {
                RepoCreate(GetCurrentWorkPath());
                printf("%d", gitState);
            }
            else if (argc >= 3 && strcmp(argv[2], "help") == 0)
            {
                printf("Provide content of repository objects.\nUsage:\ncat-file TYPE[blob/commit/tag/tree] OBJECT\n");
            }
            else if (argc == 4)
            {
                char *fmt[4] = {"blob",
                                "commit",
                                "tag",
                                "tree"};
                int type = 0;
                if (strcmp(argv[2], fmt[0]) == 0)
                    type = 0;
                else if (strcmp(argv[2], fmt[1]) == 0)
                    type = 1;
                else if (strcmp(argv[2], fmt[2]) == 0)
                    type = 2;
                else if (strcmp(argv[2], fmt[3]) == 0)
                    type = 3;
                else
                    type = -1;

                char *obj = argv[3];

                GitRep *newRep = RepoFind(NULL);
                CatFile(newRep, (void *)obj, fmt[type]);
            }
        }
        else if (strcmp(argv[1], "hash-object") == 0)
        {
            if (argc >= 3 && strcmp(argv[2], "help") == 0)
            {
                printf("Compute object ID and optionally creates a blob from a file.\nUsage:\nhash-object [-w] [-t TYPE] FILE.\n[-w]  Actually write the object into the database.\n[-t TYPE] Specify the type.\nFILE Read object from <file>.\n\n");
            }
            else if (argc == 6 && strcmp(argv[2], "-w") == 0 && strcmp(argv[3], "-t") == 0)
            {
                if (strcmp(argv[4], "commit") == 0 || strcmp(argv[4], "blob") == 0 || strcmp(argv[4], "tree") == 0 || strcmp(argv[4], "tag"))
                {
                    printf("ATTENTION!!! This command might be not work well.\n");
                    CMD_HashObject(1, argv[4], argv[5]);
                }
            }
            else
            {
                printf("Unknow command. Try to type 'RuGit --help' for more information.");
            }
        }
        else if (strcmp(argv[1], "--help") == 0)
        {
            printf("Usage:\nRuGit [init/cat-file/hash-object] [...]\n");
            printf("init [name(option)]     Initialise a RuGit Repo. If this parameter is filled in, a folder named with 'name' will be generated in the current directory, and the initialisation will be done in that folder, otherwise the initialisation will be done directly in the current directory.\n\n");
            printf("cat-file TYPE[blob/commit/tag/tree] OBJECT      Provide content of repository objects.\nTYPE[blob/commit/tag/tree]  Specify the type.\nOBJECT   The object to display.\n\n");
            printf("hash-object [-w] [-t TYPE] FILE.    Compute object ID and optionally creates a blob from a file.\n[-w]  Actually write the object into the database.\n[-t TYPE] Specify the type.\nFILE Read object from <file>.\n\n");
            printf("Use 'RuGit [SubCommand] help' for Subcommand help.\n");
        }
        else if (strcmp(argv[1], "log") == 0 || strcmp(argv[1], "ls-tree") == 0 || strcmp(argv[1], "checkout") == 0 || strcmp(argv[1], "show-ref") == 0 || strcmp(argv[1], "rev-parse") == 0 || strcmp(argv[1], "ls-files") == 0 || strcmp(argv[1], "check-ignore") == 0 || strcmp(argv[1], "status") == 0 || strcmp(argv[1], "add") == 0 || strcmp(argv[1], "rm") == 0 || strcmp(argv[1], "commit") == 0)
        {
            printf("Sorry. This IS TODO WORK...\nI have tried my best:(");
        }
        else
        {
            printf("'%s' Unknow command. Try to type 'RuGit --help' for more information.", argv[1]);
        }
    }
}