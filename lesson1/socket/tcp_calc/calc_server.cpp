#include<sstream>
#include"tcp_thread_server.hpp"

int main()
{
    TcpThreadServer server;
    server.Start("0.0.0.0",9090,[](const string& req, string*resp){
            //1.反序列化
            stringstream ss(req);
            int num1=0;
            int num2=0;
            string op;
            //stringstream定义出ss，用用户输入的请求req构造ss，ss是按照空格来区分块的，把每一个操作数和运算符都放到相应的变量中 
            ss>>num1;
            ss>>num2;
            ss>>op;
            printf("req:%d  %s  %d =",num1,op.c_str(),num2);

            //2.进行计算
            int ret=0;
            if(op=="+")
            {
                ret=num1+num2;
            }
            else if(op=="-")
            {
                ret=num1-num2;
            }
            else if(op=="*")
            {
                ret=num1*num2;
            }
            else if (op=="/"&&num2!=0)
            {
                ret=num1/num2;
            }
            else if(op=="/" && num2==0)
            {
                printf("除数不能为0\n");
            }

            //3.把结果序列化
            //*resp =to_string(ret);
            stringstream ss_resp;
            ss_resp << ret;
            *resp=ss_resp.str();
            printf("%d\n",ret);
            });
    return 0;
}
