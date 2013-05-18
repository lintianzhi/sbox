#ifndef _LANG_C_H
#define _LANG_C_H
#include "lang_base.h"

class LangC : public LangBase
{
public:
    int Compile();
    int SetAllowedCall();
    void Run();
};


#endif
