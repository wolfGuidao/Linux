#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("tmp.txt", O_RDWR | O_CREAT, 0664);
    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    printf("fd = [%d]\n", fd);
    const char* lp = "linux-60";
    int ret = write(fd, lp, strlen(lp));
    printf("ret = [%d]\n", ret);

    lseek(fd, 0, SEEK_SET);

    char arr[1024] = {0};
    ret = read(fd, arr, sizeof(arr) - 1);
    printf("read ret = [%d][%s]\n", ret, arr);

    close(fd);
    return 0;
}
