#include <stdio.h>
#include <unistd.h>

int main()
{
    //程序替换
    //execl("/usr/bin/ls", "ls", "-l", NULL);
    //execlp("./getargv", "getargv", "-a", "-b", NULL);
    char* envp[3] = {0};
    envp[0] = "PATH=1111111111";
    envp[1] = "MYENV=hehehe";
    envp[2] = NULL;
    //execle("./getargv", "getargv", "-a", "-b", NULL, envp);
    char* argv[3] = {0};
    argv[0] = "getargv";
    argv[1] = "-a";
    argv[2] = NULL;
    //execv("./getargv", argv);
    execvp("./getargv", argv);
    perror("execlp");
    printf("hahahah\n");
    return 0;
}
