#include <stdio.h>
#include <unistd.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret != 0)
    {
        perror("pipe");
        return 0;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        return 0;
    }
    else if(pid == 0)
    {
        //child
        sleep(5);
        close(fd[0]);
        write(fd[1], "hehe", 4);
    }
    else
    {
        //father
        close(fd[1]);
        char buf[1024] = {0};
        read(fd[0], buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }


    return 0;
}
