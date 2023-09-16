#include "FileIO.h"
#include "GitObject.h"
#include "GitRepo.h"
#include "GitZlib.h"
#include "UnsolvedF.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("RuGit. Va0.1\nCreated by Chafiprc.");
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
                printf("Where to create the repository.");
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
            if (argc == 2)
            {
                // printf("%s\n", GetCurrentWorkPath());
                RepoCreate(GetCurrentWorkPath());
                printf("%d", gitState);
            }
            else if (argc >= 3 && strcmp(argv[2], "help") == 0)
            {
                printf("Provide content of repository objects.");
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

                GitRep *newRep = RepoFind();
                CatFile(newRep, (GitObj *)obj, Encode(fmt[type]));
            }
        }
    }
    // PathProperty("D:\\SoftLib\\Code\\Project\\RuGit\\.git\\config", &s);
    // ReadDirContent("D:\\SoftLib");
    // RepoCreate("C:\\Users\\chafiprc\\Desktop\\newGitRepo");
    // printf("%d", gitState);
    // printf("%d", s);
}