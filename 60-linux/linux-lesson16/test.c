#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int g_val = 0;

void sigcallback(int sig)
{
    g_val++;
    printf("sig : [%d]\n", sig);
}

int main()
{
    signal(2, sigcallback);

    int count = 100;
    while(count--)
    {
        printf("count : %d\n", count);
        g_val++;
        usleep(100000);
    }
    printf("g_val : %d\n", g_val);
    return 0;
}
