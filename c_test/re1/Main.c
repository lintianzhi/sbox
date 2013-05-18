#include <stdio.h>

int main()
{
    char s[1024];
    puts("start");
    while(gets(s))
        puts(s);
    int a = 1,  b = 1;
    b --;
    a = a / b;
    printf("%d", a);
}
