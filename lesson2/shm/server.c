#include "comm.h"

int main()
{
  //调用函数得到共享内存表示码
  int shmid = createShm(4096);

  //调用该函数，将进程和共享内存绑在一起，返回一个指向共享内存第一个快的指针
  char* addr = (char*)shmat(shmid,NULL,0);

  sleep(2);
  int i = 0;
  while(i++ < 26)
  {
    printf("client say : %s\n",addr);
    sleep(1);
  }

  //将共享内存和当前进程脱离
  shmdt(addr);

  sleep(2);
  //删除共享内存
  destroyShm(shmid);
  return 0;
}
