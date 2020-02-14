#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    printf("hehe\n");









    if(pid == 0)
    {
        //child
        printf("child\n");
        sleep(3);
        exit(2);
    }
    else if(pid > 0)
    {
        //father
        //WNOHANG -- waitpid 为非阻塞模式
        int status;
        while(waitpid(pid, &status, WNOHANG) == 0)
        {
            sleep(1);
            printf("father is waitpid now\n");
        }

        printf("%d\n", (status >> 8) & 0xff);
        while(1)
        {
            ;
        }
    }
    return 0;
}
