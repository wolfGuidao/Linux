#include "pthreadpool.hpp"

int main()
{
  MyTask task[10];

  MyThreadPool pool;
  for(int i = 0;i < 10;i++)
  {
    task[i].SetData(i);
    pool.AddTaskToPool(&task[i]);
  }

  pool.StopThreadPool();

  return 0;
}
