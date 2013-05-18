#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char *argv[])
{

    printf("%s\n", argv[0]);
    pid_t pid = fork();
    if(pid == 0)
    {
//        freopen("in", "r", stdin);
//        freopen("out", "w", stdout);

        int fdi = open("in", O_RDONLY);
        int fdo = open("out", O_WRONLY);
        dup2(fdi, 0);
        dup2(fdo, 1);
        close(fdi);
        close(fdo);

        execl("./Main", "./Main", (char *)NULL);
    }
    return 0;
}
