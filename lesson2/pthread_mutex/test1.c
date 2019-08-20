#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int ticket = 100;//全局共享变量

void* route(void* arg)
{
  char* id = (char*)arg;
  while(1)
  {
    if(ticket > 0)
    {
      usleep(1000);
      printf("%s sells ticket: %d \n",id,ticket);
      ticket--;
    }
    else
    {
      break;
    }
  }
  return NULL;
}

int main()
{
  pthread_t t1,t2,t3,t4;
  pthread_create(&t1,NULL,route,(void*)"thread 1 is ok!");
  pthread_create(&t2,NULL,route,(void*)"thread 2 is ok!");
  pthread_create(&t3,NULL,route,(void*)"thread 3 is ok!");
  pthread_create(&t4,NULL,route,(void*)"thread 4 is ok!");

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_join(t3,NULL);
  pthread_join(t4,NULL);


}
