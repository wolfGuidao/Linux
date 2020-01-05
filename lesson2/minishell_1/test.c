#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char g_command[1024];

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

//进行程序替换
int exec(char** argv)
{
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
  }
  else
  {
    //father
    waitpid(pid,NULL,0);
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
    char** argv = DealCommand(g_command);

    //进行替换
    exec(argv);
  }

  return 0;
}
