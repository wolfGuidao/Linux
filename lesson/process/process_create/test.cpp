#include<iostream>
#include<stdio.h>
#include<unistd.h>
using namespace std;

int main()
{
    int pid =fork();
    int count=0;
    if(pid<0)
    {
        perror("fork");
        return 1;
    }
    else if(pid==0)
    {
        printf("child\n");
        printf("count=%d\n",++count);
    }
    else if(pid>0)
    {
        printf("father\n");
        printf("count=%d\n",++count);
    }

    return 0;
}
