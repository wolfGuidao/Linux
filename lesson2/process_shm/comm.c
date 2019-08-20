#include "comm.h"

static int commShm(int size,int flags)
{
  //产生一个key给shmget函数使用
 key_t _key = ftok(PATHNAME,PROJ_ID);
 if(_key < 0)
 {
   perror("ftok is perror");
   return -1;
 }

 int shmid = 0;
 //创建一个共享内存，返回一个共享内存的标识码
 if((shmid = shmget(_key,size,flags)) < 0)
 {
   perror("shmget is error");
   return -2;
 }
 
 return shmid;
}

int destroyShm(int shmid)
{
  //控制共享内存，这里是IPC_RMID，即删除共享内存
  if(shmctl(shmid,IPC_RMID,NULL) < 0)
  {
    perror("shmctl is error");
    return -1;
  }
  return 0;
}

int createShm(int size)
{

  return commShm(size,IPC_CREAT | IPC_EXCL | 0666);
}

int getShm(int size)
{
  return commShm(size,IPC_CREAT);
}
