#include "lang_cpp.h"
#include "lang_c.h"
#include "lang_base.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

int LangCPP::Compile()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        const char* CP_ARG[] = {"g++", "Main.c", "-o", "Main", "-O2", "-Wall", 
                                "-lm", "--static", "--std=c99", "-DONLINE_JUDGE", NULL};
        freopen("Compile_Error", "w", stderr);
        // no need to set Compile Limit
        execvp("g++", (char *const*)CP_ARG);
    }
    int status;
    waitpid(pid, &status, 0);
    if(status != 0)
    {
        iOJ_flag = OJ_CE;
        return -1;
    }
    return 0;
}

