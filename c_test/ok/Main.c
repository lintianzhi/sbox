#include <stdio.h>

int main()
{
    char s[1024];
    puts("start");
    while(gets(s))
        puts(s);
}
