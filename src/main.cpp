#include <cstdio>
#include "lang_base.h"
#include "lang_c.h"
#include "lang_cpp.h"
#include "lang_java.h"


int main(int argc, const char *argv[])
{
    if(argc < 3)
    {
        printf("Need DIR\n");
        return 1;
    }
    LangBase *langexe;
    LangType lang = (LangType)(argv[1][0] - '0');
#ifdef DEBUG
    printf("LANG: %d\n", (int)lang);
#endif
    switch(lang)
    {
        case LANG_C:
            langexe = new LangC;
            break;
        case LANG_CPP:
            langexe = new LangCPP;
            break;
        case LANG_JAVA:
            langexe = new LangJAVA;
            break;
        default:
            langexe = NULL;
    }
    if(langexe)
    {
        langexe -> Start(argv[2]);
        langexe -> PrintRst();
        delete langexe;
    }
    else
    {
#ifdef DEBUG
        printf("No such lang: %d\n", (int)lang);
#endif
    }

    return 0;
}
