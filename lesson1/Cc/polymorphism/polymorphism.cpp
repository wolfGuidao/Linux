#include<iostream>
using namespace std;

#if 0
class persson
{

public:
  public:
    int _a;
};

class student :virtual public persson
{
  public:
    void GetA()
    {
      _a++;
    }
  public:
    int _b;
};

class teacher:virtual public persson
{
  public:
    void GetA()
    {
      _a++;
    }
  public :
    int _c;
};

class D:public student,public teacher 
{
public:
#if 1
  D(int a,int b,int c,int d)
    :persson()
    ,student()
     ,teacher()
     ,_d(d)
  {

  }
#endif


  public:
    int _d;
};

int main()
{
  D d;

  student stu;
  stu.GetA();
  teacher tea;
  tea.GetA();
  cout<<stu._a<<tea._a<<d._a<<endl;

  return 0;
}
#endif

#if 0
class A
{
  public:
    int _a;
};

class B:public A
{
  public:
    int _b;
};

class C:public A
{
  public:
    int _c;
};

class D :public B,public C
{
  public:
    int _d;
};

int main()
{
  D d;
  d.B::_a=1;
  d.C::_a=2;
  d._b=3;
  d._c=4;
  d._d=5;
  cout<<d.B::_a<<endl;
  cout<<sizeof(d)<<endl;
  return 0;
}

#endif

#if 1
class A
{
  public:
    int _a;
};

class B: virtual public A
{
  public:
    int _b;
};

class C: virtual public A
{
  public:
    int _c;
};

class D :public B,public C
{
  public:
    int _d;
};

int main()
{
  D d;
  d.B::_a=1;
  d.C::_a=2;
  d._b=3;
  d._c=4;
  d._d=5;
  cout<<d.B::_a<<endl;
  cout<<sizeof(d)<<endl;
  cout<<sizeof(int)<<endl;
  cout<<sizeof(int*)<<endl;
  return 0;
}
#endif
