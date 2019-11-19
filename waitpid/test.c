#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
using namespace std;

void myhandler(int signal)
{
  cout<<signal<<endl;
  //void (signal);
  pid_t id=0;
  while(1)
  {
  id=waitpid(-1,NULL,WNOHANG);
    if(id>0)
    {
      //cout<<"succesc child "<<endl;
    }
    else 
    {
      break;
    }
  cout<<"child is overv"<<endl;
}
}
int main()
{
  //signa l(SIGCHLD,SIG_IGN);
  signal(SIGCHLD,myhandler);
  for(int i=0;i<20;i++)
  {
    int ret=fork();
    if(ret==0)
    {
      printf("child pid id %d\n",getpid());
      exit(0);
    }
  }
  while(1)
  {
    sleep(1);
  }
  return 0;
}
