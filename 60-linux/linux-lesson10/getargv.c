#include <stdio.h>

int main(int argc, char* argv[], char* env[])
{
    int i = 0;
    for(; i < argc; i++)
    {
        printf("[%d]=[%s]\n", i, argv[i]);
    }

    for(i = 0; env[i]; i++)
    {
        printf("[%d]-[%s]\n", i, env[i]);
    }
    return 0;
}
