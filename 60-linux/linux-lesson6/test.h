#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#define STUCOUNT 10
using namespace std;

//struct StuInfo
//{
//    string schoolname;
//    string classname;
//    string stuname;
//    string sex;
//};


class Person
{
public:
    Person();
    ~Person();

    int SetValue(const string& sex);
    string& GetValue();
private:
    string sex_;
};

class StuInfo : public Person
{
public:
    StuInfo();
    ~StuInfo();

    int OnInit(const char* schoolname, char* classname, const char* stuname, const string& sex);
    void printInfo();
private:
    char* schoolname1_;
    char* classname1_;
    char stuname1_[STUCOUNT];
};



