#include <stdio.h>

int k[10000000];
int main()
{
    k[99999] = 1;
    char s[1024];
    puts("start");
    while(gets(s))
        puts(s);
}
