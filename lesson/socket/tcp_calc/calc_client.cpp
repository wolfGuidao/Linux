#include<sstream>
#include"tcp_client.hpp"

int main()
{
    TcpClient client;
    bool ret = client.Connect("192.168.221.136",9090);
    if(!ret)
    {
        return 1;
    }
    while(1)
    {
        printf("请输入两个操作数num1:num2及操作符：\n");
        fflush(stdout);
        int num1=0,num2=0;
        char op[10]={0};
        scanf("%d%d%s",&num1,&num2,op);
        printf("请输入操作符op:\n");

        //序列化数据
        stringstream ss; 
        ss<<num1<<' '<<num2<<' '<<op;

        //给服务器发送数据
        client.Send(ss.str());
        string resp;
        client.Recv(&resp);

        printf("resp:%s\n",resp.c_str());
    }
    return 0;
}
