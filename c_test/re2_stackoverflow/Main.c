#include <stdio.h>

int fib(int n)
{
    if(n == 2 || n == 1)
        return 1;
    int k = fib(n-1) + 3;
    printf("%d\n", k);
    return k;
}

int main()
{
    char s[1024];
    puts("start");
    while(gets(s))
        puts(s);
    printf("%d\n", fib(300000));
}
