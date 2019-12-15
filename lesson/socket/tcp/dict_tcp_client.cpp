#include"tcp_client.hpp"

int main(int argc,char*argv[])
{
    if(argc!=2)
    {
        printf("Usage ./dict_client [ip]\n");
        return 1;
    }
    TcpClient client;
    bool ret = client.Connect(argv[1],9090);
    if(!ret)
    {
        printf("connect faile!\n");
        return 1;
    }
    while(1)
    {
    //1.从标准输入读入数据
    
        printf("请输入要查询的单词：\n");
        fflush(stdout);
        char req[1024*10]={0};
        scanf("%s",req);

    //2.把读到的数据发送给服务器
        client.Send(req);
        string resp;
        
    //3.读取服务器的响应结果
    
        client.Recv(&resp);

    //4.把响应结果显示到标准输出上
        printf("resr: %s\n",resp.c_str());
    }   
}
