#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char g_command[1024];
static char*  pipe_command[] = {0};

//该函数的功能是获取命令行输入的数据
int GetCommand()
{
  //因为开始获得的内存中的内容是不定的，所以使用前要先初始化
  memset(g_command,'\0',sizeof(g_command));
  //这里需要为'\0'留一个位置，不能把g_command全部用来读取内容,否者就没有结束标志，容易发生内存访问越界
  if(fgets(g_command,sizeof(g_command)-1,stdin)==NULL)
  {
    printf("fgets is error!\n");
    return -1;
  }
  //printf("g_command:%s\n",g_command);
  return 0;
}

//解析字符串
char** DealCommand(char* command)
{
  if(command==NULL&&*command=='\0')
  {
    printf("dealcommand is error\n");
    return NULL;
  }

  //用来保存命令
  static char* argv[1024]={0};
  int argc=0;
  while(*command)
  {
    //isspace函数用来去掉多余的空格
    //isspace的返回值：如果当前字符是空格返回1，否则返回0
    //注意'\0'在issapce函数中不算空格的，所以要进行判断
    while(!isspace(*command)&&*command!='\0')
    {
      argv[argc]=command;
      argc++;

      //去找下一个空格
      while(!isspace(*command)&&*command!='\0')
      {
        command++;
      }
      *command='\0';
    }
    command++;
  }
  argv[argc]=NULL;

  //for(int i=0;i<argc;i++)
  //{
  //  printf("%d:%s   ",i,argv[i]);
  //}
  //printf("\n");

  return argv; 
}

//进行重定向
int redirect(char * g_command)
{
  char* ptr = g_command;
  char* file = NULL;
  int fd ;
  //用来标记是清空重定向还是追加重定向
  int redirect_type = -1;

  while(*ptr !='\0')
  {
    //如果当前字符是 > ，把他置为'\0',并判断下一个位置是否为'\0'
    if(*ptr == '>')
    {
      *ptr++ = '\0';
      redirect_type++;
      if(*ptr == '>')
      {
        *ptr++ = '\0';
        redirect_type++;
      }
      //去掉多余的空格
      while(isspace(*ptr))
      {
        ptr++;
      }
      //file就是空格后面的第一个字符串
      file = ptr;

      //继续找空格，在这两个空格之间就是文件的名称
      while(!isspace(*ptr)&&*ptr != '\0')
      {
        ptr++;
      }

      *ptr='\0';
      //如果redirect_type==0说明是清空重定向,如果==1说明是追加重新定向
      if(redirect_type == 0)
      {
        fd = open(file,O_CREAT|O_TRUNC|O_WRONLY,0664);
      }
      else
      {
        fd = open(file,O_CREAT|O_APPEND|O_WRONLY,0664);
      }
      dup2(fd,1);
    }
    ptr++;
  }
  return 0;
}

//解析字符串，获取管道的数量
int do_command(char * g_command)
{
  int pipe_num = 0;
  char * ptr =g_command;

  pipe_command[pipe_num]=ptr;
  while(*ptr != '\0')
  {
    if(*ptr == '|')
    {
      pipe_num++;
      *ptr++ = '\0';
      pipe_command[pipe_num] = ptr;
      continue;
    }
    ptr++;
  }
  pipe_command[pipe_num + 1] = NULL;
  return pipe_num;
}

//进行程序替换
int exec()
{
  redirect(g_command);
  char** argv=DealCommand(g_command);
  pid_t pid =fork();
  if(pid<0)
  {
    printf("foek is error!\n");
    return -1;
  }
  else if(pid==0)
  {
    //child
    //如果argc中为NULL，就直接返回
    if(argv[0]==NULL)
    {
      exit(-1);
    }
    //进行替换，execvp第一个参数是可执行程序名，第二个参数是该可执行程序的参数组成的数组
    execvp(argv[0],argv);
    //execl("/usr/bin/ls","ls","-a",NULL);
  }
  else
  {
    //father
    waitpid(pid,NULL,0);
  }
  return 0;
}

//添加管道功能
int do_pipe(int pipe_num)
{
  int pid = 0;
  int i;
  int pipefd[10][2] = {{0}};
  char** argv = {NULL};

  for (i = 0;i <= pipe_num;i++)
  {
    pipe(pipefd[i]);
  }

  for(i = 0;i <= pipe_num; i++)
  {
    pid = fork();
    if(pid < 0)
    {
      perror("fork is error!\n");
      continue;
    }
    else if(pid == 0)
    {
      redirect(pipe_command[i]);
      argv = DealCommand(pipe_command[i]);
      //这里进行判断的目的是，如果为管道第一个，就不能够冲管道中读取数据，因为管道中还没有数据，同理，如果是最后一个，说明管道已经满了，不能够继续往管道重写数据了
      if(i != 0)
      {
        close(pipefd[i][1]);
        dup2(pipefd[i][0],0);
      }
      if(i != pipe_num)
      {
        close(pipefd[i+1][0]);
        dup2(pipefd[i+1][1],1);
      }
      execvp(argv[0],argv);
    }
    else
    {
      close(pipefd[i][0]);
      close(pipefd[i][1]);
      waitpid(pid,NULL,0);
    }

  }
  return 0;
}

int main()
{
  //循环读数据
  while(1)
  {
    printf("[dev@localhost dev]$ ");
    int ret = GetCommand();
    if(ret == -1)
    {
      //如果读取失败，继续循环读取，不能直接break调
      continue;
    }

    //处理解析数据
    //char** argv = DealCommand(g_command);

    //进行替换
    //exec(argv);
    //exec();
    int num = do_command(g_command);
    do_pipe(num);
  }

  return 0;
}
