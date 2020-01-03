#include<iostream>
using namespace std;

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
