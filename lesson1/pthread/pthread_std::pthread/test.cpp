#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;

#include<thread>
#include<string>

//原子操作,本质上也是锁，是指令级别的锁
#include<atomic>
//C++中的std::thread


atomic_int count(0);
//int count=0;
void Handler()
{
    for(int i=0;i<50000;i++)
    {
        ++count;
        //printf("count = %d\n",count);
    }
}

int main()
{
    //构造函数采用变长参数模板
    //thread t1(Handler,999,"wolf guidao");
    //thread t2(Handler,666,"love you");
    //t1.join();
    //t2.join();
    thread t1(Handler);
    thread t2(Handler);
    t1.join();
    t2.join();
    printf("wolf count = %d\n",(int)count);
    return 0;
}
