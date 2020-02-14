#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 0;
    }
    else if(pid == 0)
    {
        //child
        printf("i am child - %d\n", getpid());
        //int* lp = NULL;
        //*lp = 10;
        sleep(3);
        exit(88);
    }
    else
    {
        //father
        //pid_t wait(int* status);
        pid_t ret = fork();
        if(ret == 0)
        {
            //child
            printf("i am child2 - %d\n", getpid());
            exit(1);
        }

        int status;
        wait(&status);
        printf("coredump code = %d\n", (status >> 7) & 0x1);
        printf("sig code = %d\n", status & 0x7f);
        //printf("%d\n", (status >> 8) & 0xff);
        sleep(20);
        while(1)
        {
            printf("我在打麻将~\n");
        }
    }
    return 0;
}
