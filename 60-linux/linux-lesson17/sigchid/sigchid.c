#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void sigcallback(int sig)
{
    printf("sig : %d\n", sig);
    wait(NULL);
}

int main()
{
    signal(SIGCHLD, sigcallback);
    pid_t pid = fork();
    if(pid < 0)
    {
        return pid;
    }
    else if(pid == 0)
    {
        //child
        printf("i am child\n");
        sleep(3);
        exit(1);
    }
    else
    {
        //wait
        while(1)
        {
            printf("我不听~~~\n");
            sleep(1);
        }
    }
    return 0;
}
