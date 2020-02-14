#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


char g_Command[1024];

int GetCommand()
{
    memset(g_Command, '0', sizeof(g_Command));
    printf("[minishell@localhost]$ ");
    fflush(stdout);
    if(!fgets(g_Command, sizeof(g_Command) - 1, stdin))
    {
        perror("fgets");
        return -1;
    }
    //printf("g_Command:%s\n", g_Command);
    return 0;
}

int ExecCommand(char* argv[])
{
    if(!argv[0])
    {
        return -1;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return -1;
    }
    else if(pid == 0)
    {
        //child
        execvp(argv[0], argv);
        exit(0);
    }
    else
    {
        //father
        waitpid(pid, NULL, 0);
    }
    return 0;
}

int DealCommand(char* command)
{
    if(!command || *command == '\0')
    {
        printf("DealCommand failed\n");
        return -1;
    }

    //strtok strchr
    char* argv[32] = {0};
    int argc = 0; 
    while(*command)
    {
        if(!isspace(*command))
        {
            argv[argc] = command;
            argc++;
            while(!isspace(*command) && *command != '\0')
            {
                command++;
            }
            *command = '\0';
        }
        command++;
    }

    argv[argc] = NULL;
    //for(int i = 0; i < argc; i++)
    //{
    //    printf("[%d]:[%s]\n", i , argv[i]);
    //}
    ExecCommand(argv);
    return 0;
}

int main()
{
    while(1)
    {
        if(GetCommand() != 0)
        {
            continue;
        }
        //切割命令和命令行参数，在程序替换当前的命令
        DealCommand(g_Command);
    }
    return 0;
}
