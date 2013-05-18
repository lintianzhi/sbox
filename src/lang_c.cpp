#include "lang_c.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

int LangC::Compile()
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        const char* CP_ARG[] = {"gcc", "Main.c", "-o", "Main", "-O2", "-Wall", 
                                "-lm", "--static", "--std=c99", "-DONLINE_JUDGE", NULL};
        freopen("Compile_Error", "w", stderr);
        // no need to set Compile Limit
        execvp("gcc", (char *const*)CP_ARG);
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

int LangC::SetAllowedCall()
{
    memset(iAllowCall, 0, sizeof(iAllowCall));
#ifdef DEBUG
        printf("Allow_C:");
#endif
    for(int i=0;Allow_C[i];i++)
    {
        iAllowCall[Allow_C[i]] = 1;
#ifdef DEBUG
        printf(" %d", Allow_C[i]);
#endif
    }
#ifdef DEBUG
        printf("\n");
#endif
    return 0;
}

void LangC::Run()
{
    // already freopen && set limit
    if(execl("./Main", "./Main", (char *)NULL) == -1)
    {
        printf("execl fail, errno = %d, %s\n", errno, strerror(errno));
    }
}
