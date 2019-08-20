#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define key 0x123456

int main()
{
  int shmid = shmget(key,1024,IPC_CREAT);
  if (shmid < 0)
  {
    perror("shmget is error\n");
    return -1;

  }

  char* addr = (char*)shmat(shmid,NULL,0);

  while(1)
  {
    //read(0,addr,sizeof(addr));
    //scanf("please entry:%s",addr);
    write(0,addr,10);
  }

  shmdt(addr);
  shmctl(shmid,IPC_RMID,NULL);
  return 0;
}

