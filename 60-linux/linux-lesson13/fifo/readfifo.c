#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    //如果单单只是执行了open，在内核当中创建了没有缓冲区
    int fd = open("./a.fifo", O_RDWR);
    if(fd < 0)
    {
        return 0;
    }

    char buf[1024] = {0};
    read(fd, buf, sizeof(buf) - 1);
    printf("buf = [%s]\n", buf);
    while(1)
    {
        ;
    }
    return 0;
}
