# Rufish'g Own Git Project

RuGit is a project for personal practice and DianGroup Recruitment.

## Usage:

1. 解压目录下的 x86_64-13.1.0-release-win32-seh-ucrt-rt_v11-rev1.7z 并打开其中的 mingw64/include/bin 目录，将 bin 目录添加到系统环境变量，确保编译时使用的是压缩包内的 gcc
2. 后使用'gcc RuGit.c -Wall -o RuGit -lz' 进行编译

## NOTICE:

由于时间原因以及个人能力问题 完整实现的仅有

```bash
RuGit.exe init
```

代码完成但无法运行的:

```bash
RuGit.exe cat-file/hash-object
```
