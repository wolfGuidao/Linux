#include <stdio.h>

int main()
{
    union AB
    {
        int a;
        char b;
    }c;

    c.a = 1;
    if(c.b == 1)
    {
        printf("xiaoduan\n");
    }
    return 0;
}
