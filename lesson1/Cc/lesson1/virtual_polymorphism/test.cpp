#include<iostream>
using namespace std;

class A1
{
  public:
    virtual void Test1()
    {
      cout<<"A1::Test"<<endl;
    }

    int _a1;

};

class A2
{

  public:
    virtual void Test2()
    {
      cout<<"A2::Tests"<<endl;
    }

    int _a2;
};

class B:public A1,public A2
{

  public :
    virtual void Test1()
    {
      cout<<"B::Test()"<<endl;
    }

    virtual void Test2()
    {
      cout<<"B::Test"<<endl;
    }

    int _b;
};

int main()
{

  B b;
  A1 a1;
  A2 a2;
  cout<<sizeof(a1)<<endl;
  cout<<sizeof(a2)<<endl;
  cout<<sizeof(b)<<endl;
  //cout<<sizeof(int*)<<endl;
  cout<<sizeof(int)<<endl;
  return 0;
}
