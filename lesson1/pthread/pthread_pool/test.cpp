#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include"threadpool.hpp"
using namespace std;

//用户自定值，想要什么就自己定制添加就ok
class MyTask : public Task
{
public:
    MyTask(int id)
    {
        _id=id;
    }
    void Run()
    {
        //用户自定制逻辑
        printf("id = %d\n",_id);
    }
private:
    int _id;
};

int main()
{
    ThreadPool pool(10);
    for(int i=0;i<20;i++)
    {
       pool.AddTask(new MyTask(i));
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}
