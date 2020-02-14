#include <stdio.h>
#include <unistd.h>

int main()
{
    char arr[51] = { 0 }; 
    const char* lp = "|/-\\";
    for(int i = 0; i < 50; i++)
    {
        arr[i] = '#';
        printf("[%-50s][%d%%][%c]\r", arr, i + 1, lp[i % 4]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\n");
    while(1)
    {
        ;
    }
    return 0;
}
