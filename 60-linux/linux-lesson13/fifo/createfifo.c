#include <stdio.h>
#include <sys/stat.h>

int CreateFifo()
{
    int ret = mkfifo("./b.fifo", 0664);
    if(ret < 0)
    {
        return 1;
    }
    return ret;
}


int main()
{
    CreateFifo();
    return 0;
}
