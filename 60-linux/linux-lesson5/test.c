//gcc test.c a.c -o out

#include "test.h"
#include "a.h"

void func()
{
    printf("i am test.c\n");
}

int main()
{
    func();
    func1();
    return 0;
}
