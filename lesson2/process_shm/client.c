#include "comm.h"

int main()
{
  //得到一个共享内存的标识码
  int shmid = getShm(4096);

  sleep(1);

  //将当前进程绑到共享内存上
  char* addr = (char*)shmat(shmid,NULL,0);

  sleep(2);
  int i = 0;
  while(i < 26)
  {
    addr[i] = 'A' + i;
    i++;
    addr[i] = 0;
    sleep(2);
  }

  //将当前进程和共享内存分离
  shmdt(addr);
  destroyShm(shmid);

  sleep(2);

  return 0;
}
