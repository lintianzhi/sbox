#include "lang_base.h"
#include <sys/resource.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>


LangBase::LangBase():iUsedtime(0), iPeakmem(0), iOJ_flag(OJ_PD)
{
}
LangBase::~LangBase()
{
    if(iErrFP)
        fclose(iErrFP);
}

int LangBase::GetProcStatus(pid_t pid, const char *str)
{
    FILE *fd;
    char fname[32], line[128];
    int v, len;
    len = strlen(str);
    sprintf(fname, "/proc/%d/status", pid);
    fd = fopen(fname, "r");
    if(!fd)
    {
        return -1;
    }
    while(fgets(line, 127, fd))
    {
        if(strncmp(line, str, len) == 0)
        {
            sscanf(line+len+1, "%d", &v);
            break;
        }
    }
    fclose(fd);
    return v;
}

int LangBase::ReadRuntimeLim()
{
    int time_l, mem_l, stack_l, file_l;
    FILE *fp = fopen("limit", "r");
    if(!fp)
    {
#ifdef DEBUG
        printf("Cannot open file : limit\n");
        printf("errno = %d, means: %s\n", errno, strerror(errno));
#endif
        return -1;
    }
    fscanf(fp, "%d%d%d%d", &time_l, &mem_l, &stack_l, &file_l);
    fclose(fp);
   
    time_l  *= 1;
    mem_l   *= STD_MB;
    stack_l *= STD_MB;
    file_l  *= STD_MB;
#ifdef DEBUG
    printf("Time limit:   %d\nMemory limit: %d B\nStack limit:  %d B\nFile limit:   %d B\n",
            time_l, mem_l, stack_l, file_l);
#endif

    cpu_lim.rlim_max   = cpu_lim.rlim_cur   = time_l;
    cpu_lim.rlim_max ++;  // for detect OJ_TL
    mem_lim.rlim_max    = mem_lim.rlim_cur    = mem_l;
    iMaxMem = mem_l / 1024;
    stack_lim.rlim_max  = stack_lim.rlim_cur  = stack_l;
    file_lim.rlim_max   = file_lim.rlim_cur   = file_l;
    thread_lim.rlim_max = thread_lim.rlim_cur = 1;
    return 0;
}

int LangBase::SetRuntimeLim()
{
    //  if -1 is returned, see errno
#ifdef DEBUG
    printf("SetRuntimeLim\n");
#endif
    if(setrlimit(RLIMIT_STACK, &stack_lim) != 0)
        return -1;
    if(setrlimit(RLIMIT_CPU, &cpu_lim) != 0)
        return -1;
    if(setrlimit(RLIMIT_DATA, &mem_lim) != 0)
        return -1;
//   if(setrlimit(RLIMIT_AS, &mem_lim) != 0)
//       return -1;
    if(setrlimit(RLIMIT_FSIZE, &file_lim) != 0)
        return -1;
    if(setrlimit(RLIMIT_NPROC, &thread_lim) != 0)
        return -1;
#ifdef DEBUG
    printf("SetRuntimeLim return\n");
#endif
    return 0;
}

int LangBase::GetVmPeak(pid_t pid, struct rusage *rsc)
{
    return GetProcStatus(pid, "VmPeak");
}

void LangBase::Monitor(pid_t pid)
{
    struct rusage rsc;

    iOJ_flag = OJ_PD;
    iUsedtime = iPeakmem = 0;
    while(1)
    {
        int status;
        wait4(pid, &status, 0, &rsc);
//        int tmp = GetProcStatus(pid, "VmPeak");
        int tmp = GetVmPeak(pid, &rsc);
        if(tmp > iPeakmem)
            iPeakmem = tmp;
        if(iPeakmem > iMaxMem)
        {
#ifdef DEBUG
   printf("memory exceeded, Peak memory: %d, allowed memory: %d\n", iPeakmem, iMaxMem); 
#endif
            iOJ_flag = OJ_ML;
            ptrace(PTRACE_KILL, pid, NULL, NULL);
            break;
        }
        else if(WIFEXITED(status))
        {
            // exit normally
#ifdef DEBUG
            printf("EXIT normally\n");
#endif
            iOJ_flag = OJ_TC;
            break;
        }
        
        else if(WIFSTOPPED(status))
        {
            // Stoped, receive a signal that not be catched
            int stopedsig = WSTOPSIG(status);
            if(stopedsig != SIGTRAP) // Trace trap (POSIX)
            {
                switch(stopedsig)
                {
                    case SIGSEGV:
                        iOJ_flag = OJ_ML;  // set it to OJ_STK
                        break;
                    case SIGXCPU:
                        iOJ_flag = OJ_TL;
                        break;
                    case SIGXFSZ:
                        iOJ_flag = OJ_OL;
                        break;
                    case SIGFPE:
                        iOJ_flag = OJ_FPE;
                        break;
                    case SIGKILL:
                    default:
                        iOJ_flag = OJ_RE;
                }
#ifdef DEBUG
                printf("Stoped signal: %d\n", stopedsig);
#endif
                ptrace(PTRACE_KILL, pid, NULL, NULL);
                break;
            }
        }
        else if(WIFSIGNALED(status))  // terminated by signal
        {
            int sig = WTERMSIG(status);
            switch(sig)
            {
                case SIGSEGV:
                    iOJ_flag = OJ_ML;
                    break;
                case SIGXCPU:
                    iOJ_flag = OJ_TL;
                    break;
                case SIGXFSZ:
                    iOJ_flag = OJ_OL;
                    break;
                case SIGKILL:
                default:
                    iOJ_flag = OJ_RE;
            }
#ifdef DEBUG
            printf("Terminal Signal: %s\n", strsignal(sig));
#endif
            break;
        }
        {
            struct user_regs_struct reg;
            ptrace(PTRACE_GETREGS, pid, NULL, &reg);
#ifdef DEBUG
                printf("Process executed syscall: %ld\n", reg.orig_eax);
#endif
            if(iAllowCall[reg.orig_eax] == 0)
            {
                ptrace(PTRACE_KILL, pid, NULL, NULL);
                iOJ_flag = OJ_RE;
#ifdef DEBUG
                fprintf(stdout, "[RE] Not allowed syscall, %ld\n", reg.orig_eax);
#else
                fprintf(iErrFP, "[RE] Not allowed syscall, %ld\n", reg.orig_eax);
#endif
                break;
            }
            ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
        }
    }
    iUsedtime  = (rsc.ru_utime.tv_sec * 1000 + rsc.ru_utime.tv_usec / 1000);
    iUsedtime += (rsc.ru_stime.tv_sec * 1000 + rsc.ru_stime.tv_usec / 1000);

    return;
}

// chdir to temp dir.
void LangBase::Start(const char *dir)
{
    chdir(dir);
    strncpy(iDir, dir, sizeof(iDir)-1);
    int dirlen = strlen(iDir);
    if(iDir[dirlen-1] == '/')
        iDir[dirlen-1] = '\0';

#ifdef DEBUG
    printf("Read Runtime limit Start\n");
#endif
    if(ReadRuntimeLim() != 0)
        return;
#ifdef DEBUG
    printf("Read Runtime limit OK\n");
#endif

    if(Compile() != 0)
    {
#ifdef DEBUG
    printf("Compile Fail\n");
#endif
        return;
    }
#ifdef DEBUG
    printf("Compile OK\n");
#endif

    iErrFP = fopen("err", "w");

    if(SetAllowedCall() != 0)
        return;
#ifdef DEBUG
    printf("Allowed syscall:  ");
    for(int i=0;i<512;i++)
        if(iAllowCall[i])
            printf(" %d", i);
    printf("\nSet Allowed Call OK\n");
#endif
    SetChrootEnv();
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
//        if(chroot(".") == -1)   //  root privilige
//        {
//#ifdef DEBUG
//            printf("chroot error, %s\n", strerror(errno));
//#endif
//            return;
//        }
        if(SetRuntimeLim() != 0)
            exit(1);
        freopen("in","r", stdin);
        freopen("out","w", stdout);
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        //exit(0);
#ifdef DEBUG
        printf("Start Runing child.\n");
#endif
        Run();  // exec
    }
    Monitor(pid);
}

void LangBase::PrintRst()
{
    FILE *fp = fopen("result", "w");
    fprintf(fp, "%d\n", (int)iOJ_flag);
    fprintf(fp, "%d\n%d\n",iUsedtime, iPeakmem);
    fclose(fp);
#ifdef DEBUG
    printf("OJ_FLAG: %d, %s\n", iOJ_flag, OJ_STATUS_NAME[iOJ_flag]);
    printf("Total usedtime: %d, peak memory: %d KB\n", iUsedtime, iPeakmem);
#endif
}

void LangBase::PrintErrMsg(char *s)
{
    fprintf(iErrFP, "%s", s);
}

const char *OJ_STATUS_NAME[OJ_NUM] = {
    "OJ_PD", "OJ_CE", "OJ_TC", "OJ_AC", "OJ_WA", 
    "OJ_PE", "OJ_TL", "OJ_ML", "OJ_OL", "OJ_RE", 
    "OJ_FPE"
};
