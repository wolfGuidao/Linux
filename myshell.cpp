#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
//intput表示待切分的命令，output表示切分结果，返回值代表output中有效元素的个数
int Split(char input[],char* output[])
{
  char* p=strtok(input," ");
  int i=0;
  while(p!=NULL)
  {
    output[i++]=p;
    p=strtok(NULL," ");
  }
  output[i]=NULL;
  return i;
}

void CreateProcess(char* argv[])
{
  //void(n);
  //1.创建子进程
  pid_t ret=fork();
  if(ret>0)
  {//father
    wait(NULL);
  }
  else if(ret==0)
  {//child
    ret=execvp(argv[0],argv);
    if(ret<0)
  //if是可以省略的，如果子进程替换成功，那么程序是一定不会走到if这个代码
    {
      perror("exec");
      exit(0);
    }
  }
  else
  {
    perror("fork");
  }
  //2.父进程进行进程等待，子进程进行程序替换
  
}

int main()
{
  while(1)
  {
    //1.打印命令行提示符
    printf("[myshell@localhost ~ ]$ ");
    fflush(stdout);
    //2.用户输入一行指令
    char command[1024]={0};
    gets(command);
    char* argv[1024];
    //3.解析用户输入的指令，识别出哪些是命令行参数，那些是程序（即字符串切分）
    int n = Split(command,argv);
    //4.创建子进程进行进程替换
   CreateProcess(argv);  

  }
  return 0;
}
