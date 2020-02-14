#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigbackfunc(int sig)
{
    printf("%d\n", sig);
}

int main()
{
    struct sigaction newact;
    struct sigaction oldact;

    newact.sa_handler = sigbackfunc;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);
    sigaction(2, &newact, &oldact);
    
    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
    return 0;
}
