#ifndef _LANG_BASE_H
#define _LANG_BASE_H

#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include "allowed_syscall.h"

enum OJ_Status
{
    OJ_PD, // pending
    OJ_CE = 1, 
    OJ_TC,  // run correct, to be compared 
    OJ_AC,
    OJ_WA,
    OJ_PE,
    OJ_TL,
    OJ_ML,
    OJ_OL,
    OJ_RE,
    OJ_FPE,
    OJ_NUM
};
extern const char *OJ_STATUS_NAME[OJ_NUM];

enum LangType
{
    LANG_C = 1,
    LANG_CPP,
    LANG_JAVA,
    LANG_PY,
    LANG_GO
};

const int STD_MB = 1048576;

class LangBase
{
public:
    LangBase();
    ~LangBase();
    static int GetProcStatus(pid_t pid, const char *str);
    virtual int Compile() {}
    virtual void SetChrootEnv(){}
    virtual int ReadRuntimeLim();
    virtual int GetVmPeak(pid_t pid, struct rusage *rus);
    int SetRuntimeLim();
    virtual int SetAllowedCall() = 0;
    virtual void Run() = 0;
    void Monitor(pid_t pid);
    void Start(const char *dir);
    void PrintRst();
    void PrintErrMsg(char *s);
protected:
    struct rlimit stack_lim, cpu_lim, mem_lim, file_lim, thread_lim;
    char iDir[16];
    OJ_Status iOJ_flag;
    int iUsedtime, iPeakmem; 
    int iMaxMem;
    FILE *iErrFP;
    int iAllowCall[512];
};

#endif
