#include <stdio.h>
#include <unistd.h>

int main()
{
    char s[1024];
    puts("start");
    while(gets(s))
        puts(s);
    freopen("kk","r",stdin);
}
