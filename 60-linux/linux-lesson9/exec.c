#include <stdio.h>
#include <unistd.h>

int main()
{
    //程序替换
    //execl("/usr/bin/ls", "ls", "-l", NULL);
    execlp("getargv", "getargv", "-a", "-b", NULL);
    printf("hahahah\n");
    return 0;
}
