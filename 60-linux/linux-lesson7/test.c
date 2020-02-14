#include <stdio.h>
#include <unistd.h>

//libc

int g_val = 10;

int main()
{
    //创建一个进程
    //fork
    //pid_t fork(void);
    pid_t pid = fork();
    if(pid < 0)
    {
        //没有创建出来子进程
        perror("fork");
        return 0;
    }
    else if(pid == 0)
    {
        //child
        g_val = 100;
        printf("child : pid = %d, ppid = %d, g_val = %d-%p\n", getpid(), getppid(), g_val, &g_val);
    }
    else//pid > 0
    {
        //parent
        printf("parent : pid = %d, ppid = %d, g_val = %d-%p\n", getpid(), getppid(),g_val, &g_val);
    }


    //getpid() -- 
    //pid_t getpid(void);
    //获取当前进程的pid的系统调用
    //pid = getpid();
    //printf("%d\n", pid);
    //while(1)
    //{
    //    ;
    //}
    return 0;
}
