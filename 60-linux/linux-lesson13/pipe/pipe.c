#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int g_val = 100;

int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret < 0)
    {
        return ret;
    }

    //如果当前把写端关闭掉，读端去读，则...
    int count = 10;
    while(count--)
    {
        write(fd[1], "a", 1);
    }
    close(fd[1]);

    char buf[1024] = {0};
    int read_size = read(fd[0], buf, sizeof(buf) - 1);
    printf("[%d]-[%s]\n", read_size, buf);

    //如果当前把读端 fd[0] 关闭，在往写端去写
    //close(fd[0]);
    //write(fd[1], "linux666", 8);
    //while(1)
    //{
    //    printf("hehehe\n");
    //}

    //设置fd[0]：读端为非阻塞
    //int flags = fcntl(fd[0], F_GETFL);
    //flags |= O_NONBLOCK;
    //fcntl(fd[0], F_SETFL, flags);

    //如果写端不写，则读端会陷入阻塞
    //fd[0] fd[1]
    //char buf[1024] = {0};
    //int ret_size = read(fd[0],buf, sizeof(buf) - 1);
    //printf("ret_size : [%d]\n", ret_size);
    
    //如果读端不读，写端一个写，则最后会将管道写满，并且进入到阻塞状态
    //while(1)
    //{
    //    write(fd[1], "1", 1);
    //}
    return 0;
}
