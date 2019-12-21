#include<iostream>
#include<thread>
#include<mutex>
#include<thread>
using namespace std;

//饿汉模式
#if 0
template<typename T>
class Singleton
{
    public:
        static T* Instance()
        {
            return &inst_;
        }
    private:
        static T inst_;
};
#endif

//懒汉模式
template<class T>
class Singleton
{
public:
   static T* Instance()
    {
        //双重判断提高程序性能
        if(inst_==NULL)
        {
            lock.lock();
            if(inst_==NULL)
            {
                inst_=new T();
            }
            lock.unlock();
        }
        return  inst_;
     }
private:
   //防止编译器把inst_优化到寄存器中
    volatile static T* inst_=NULL;;
    static mutex lock;
};


int mian()
{
    return 0;
}
