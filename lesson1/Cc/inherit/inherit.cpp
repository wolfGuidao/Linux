#include<iostream>

using namespace std;


class Base
{
  public:
  void SetBase(int pri,int pro,int pub)
  {
    _pri=pri;
    _pro=pro;
    _pub=pub;
  }

  void PrintBase()
  {
    cout<<_pri<<_pro<<_pub<<endl;
  }

  private:
    int _pri;
  protected:
    int _pro;
  public:
    int _pub;
};

//继承权限
class Derived : public  Base
{
  //访问权限，限制是否可以在类外访问
  public:
    void Init(int _a,int _b,int _c)
    {
      a=_a;
      b=_b;
      c=_c;
    }

    void Print()
    {
      cout<<a<<b<<c<<endl;
    }
  public:
    int a;
  protected:
    int b;
  private:
    int c;
};

int main()
{
  //验证继承
  Derived d;
  cout<<sizeof(Derived)<<endl;
  d.SetBase(10,20,30);
  d.PrintBase();
  
  d.Init(40,50,60);
  d.Print();
  return 0;
}
