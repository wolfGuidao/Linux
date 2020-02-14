#include <stdio.h>
#include <unistd.h>
int main()
{
    //创建子进程
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 0;
    }
    else if(pid == 0)
    {
        //child
        printf("i am child process\n");
        sleep(100);
    }
    else
    {
        //parent
        printf("i am perent process\n");
    }

    return 0;
}
