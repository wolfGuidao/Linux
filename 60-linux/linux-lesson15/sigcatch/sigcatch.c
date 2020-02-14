#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigbackfunc(int sig)
{
    printf("sig : [%d]\n", sig);
    signal(2, SIG_DFL);
}

int main()
{
    signal(SIGINT, sigbackfunc);
    while(1)
    {
        printf("hello~\n");
        sleep(1);
    }
    return 0;
}
