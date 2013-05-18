#ifndef _LANG_JAVA_H
#define _LANG_JAVA_H
#include "lang_base.h"

class LangJAVA : public LangBase
{
public:
    int ReadRuntimeLim();
    int GetVmPeak(pid_t pid, struct rusage *rsc);
    void Monitor(pid_t pid);
    int Compile();
    void SetChrootEnv();
    int SetAllowedCall();
    void Run();
};


#endif
