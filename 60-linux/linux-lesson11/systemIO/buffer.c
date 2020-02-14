#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("printf");
    const char* lp = "linux";
    const char* lp1 = "-60-fprintf";
    fprintf(stdout, "%s%s", lp, lp1);
    fwrite("fwrite", 1, 6, stdout);
    write(1, "write", 5);

    sleep(3);
    //
    return 0;
}
