#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigcallback(int sig)
{
    printf("sig : [%d]\n", sig);
}

int main()
{
    signal(2, sigcallback);
    signal(40, sigcallback);

    //block位图设置一下,也就是阻塞某些信号
    //int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
    sigset_t set, oldset;
    sigemptyset(&set);
    sigemptyset(&oldset);

    //给set位图编程全1
    //int sigfillset(sigset_t *set);
    sigfillset(&set);

    sigprocmask(SIG_BLOCK, &set, &oldset);
    getchar();
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    while(1)
    {
        ;
    }
    return 0;
}
