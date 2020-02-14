#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
//    char* lp = (char*)malloc(10);
    //1
    //free(lp);
    //free(lp);//报错？ 崩溃？

    //2
//    free(lp);
//    lp = NULL;
//    free(lp);//崩溃？
//
    //kill(getpid(), SIGINT);
    //abort();
    //raise(2);
    //alarm(3);
    int *p = NULL;
    *p = 10;
    while(1)
    {
        printf("i am sleep!\n");
        sleep(1);
    }
    return 0;
}
