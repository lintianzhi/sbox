#include "lang_base.h"
#include "lang_java.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

int LangJAVA::ReadRuntimeLim()
{
    if(LangBase::ReadRuntimeLim() < 0)
        return -1;

    // Java fucking BONUS
    cpu_lim.rlim_max *= 2;
    cpu_lim.rlim_cur *= 2;
    iMaxMem *= 2;
    mem_lim.rlim_max *= 2;
    mem_lim.rlim_cur *= 2;
    thread_lim.rlim_max = 100;
    thread_lim.rlim_cur = 100;
    return 0;
}

int LangJAVA::Compile()
{
#ifdef DEBUG
   printf("JAVA Compile Start\n"); 
#endif
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        char *CP_ARG[5];
        char buf[4][16];
        for(int i=0;i<4;i++) CP_ARG[i] = buf[i];
        sprintf(CP_ARG[0], "javac");
        sprintf(CP_ARG[1], "-J-Xms32m");
        sprintf(CP_ARG[2], "-J-Xms%dm", iMaxMem/1024);
        sprintf(CP_ARG[3], "Main.java");
        CP_ARG[4] = '\0';
        freopen("Compile_Error", "w", stderr);
        // no need to set Compile Limit
        if(execvp("javac", (char *const*)CP_ARG) == -1)
        {
            printf("Error during Compile Java FILE: %s\n", strerror(errno));
            exit(1);
        }
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

int LangJAVA::GetVmPeak(pid_t pid, struct rusage *rsc)
{
    int m_minflt;
    m_minflt = rsc->ru_minflt * getpagesize();
    m_minflt /= 1024;
#ifdef DEBUG
    int m_vmdata, m_vmpeak;
    m_vmpeak = GetProcStatus(pid, "VmPeak");
    m_vmdata = GetProcStatus(pid, "VmData");
//    printf("VmPeak: %d KB, VmData: %d KB, minflt: %d KB\n", m_vmpeak, m_vmdata, m_minflt);
#endif
    return m_minflt;
}

void LangJAVA::SetChrootEnv()
{
    
}


int LangJAVA::SetAllowedCall()
{
    memset(iAllowCall, 0, sizeof(iAllowCall));
#ifdef DEBUG
        printf("Allow_JAVA:");
#endif
    for(int i=0;Allow_JAVA[i];i++)
    {
        iAllowCall[Allow_JAVA[i]] = 1;
#ifdef DEBUG
        printf(" %d", Allow_JAVA[i]);
#endif
    }
#ifdef DEBUG
        printf("\n");
#endif
    return 0;
}

void LangJAVA::Run()
{
    freopen("err", "w", stderr);
    // already freopen && set limit
    execl("/usr/bin/java", "/usr/bin/java",
            "-Djava.security.manager",
            "-Djava.security.policy=./java.policy",
            "Main", (char *)NULL);
}

void LangJAVA::Monitor(pid_t pid)
{
    LangBase::Monitor(pid);
    if(iOJ_flag == OJ_TC)
    {
        int status = GetFileSize("err");
#ifdef DEBUG
        printf("err file size: %d\n", status);
#endif
        if(status != 0)
        {
            iOJ_flag = OJ_RE;
        }
    }
}

