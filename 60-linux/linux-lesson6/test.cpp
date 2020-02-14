#include "test.h"

Person::Person()
{
    sex_ = "";
}

Person::~Person()
{

}

int Person::SetValue(const string& sex)
{
    sex_ = sex;
    return 0;
}

string& Person::GetValue()
{
    return sex_;
}

StuInfo::StuInfo()
{
    schoolname1_ = NULL;
    classname1_ = NULL;
    memset(stuname1_, '0', STUCOUNT);
}

StuInfo::~StuInfo()
{

}

int StuInfo::OnInit(const char* schoolname, char* classname, const char* stuname, const string& sex)
{
    strcpy(schoolname1_, schoolname);
    classname1_ = classname;
    strcpy(stuname1_, stuname);
    Person::SetValue(sex);

    return 0;
}


void StuInfo::printInfo()
{
    printf("%s-%s-%s-%s\n", schoolname1_, classname1_, stuname1_, Person::GetValue().c_str());
}


int main()
{
    StuInfo* stu = new StuInfo();
    const char* schname = "bite";
    char* clasname = (char*)"60";
    char stuname[13] = "nigulasikasi";
    string sex = "male";
    stu->OnInit(schname, clasname, stuname, sex);
    stu->printInfo();

    return 0;
}
