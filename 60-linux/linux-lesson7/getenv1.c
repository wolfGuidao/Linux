#include <stdio.h>

int main(int argc, char* argv[], char* env[])
{
    //char* 等于 "xxx"
    //char* 等于 "xxx"
    //char* 等于 NULL
    int i = 0;
    for(; env[i]; i++)
    {
        printf("%s\n", env[i]);
    }
    return 0;
}
