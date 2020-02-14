#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void Func()
{
    exit(88);
}

int main(int argc, char* argv[])
{
    //异常退出
    //int* lp = NULL;
    //*lp = 10;
    //_exit(66);
    //Func();
    printf("hehe");
    return 0;
    _exit(88);
    if(argc != 2)
    {
        for(int i = 0; i < argc; i++)
        {
            printf("[%d]-[%s]\n", i, argv[i]);
        }
        return 0;
    }
    for(int i = 0; i < 256; i++)
    {
        printf("[%d]=[%s]\n", i, strerror(i));
    }
    return 99;
}
