#include "tcpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svr [ip] [port]\n");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpServer ts;
    if(!ts.CreateSock())
    {
        return 0;
    }

    if(!ts.Bind(ip, port))
    {
        return 0;
    }

    if(!ts.Listen(5))
    {
        return 0;
    }
    TcpServer peerts;
    if(!ts.Accept(peerts))
    {
        return 0;
    }
    
    //想使用TcpServer当中的收发数据接口，并且使用成员变量保存新创建(接收和发送数据)出来的socket
    while(1)
    {
        sleep(20);
        //客户端会有足够的时间进行发送，发送的数据到了服务端socket的接收缓冲区当中
        //120字节的数据 
        //40字节  -- 总共接收3次
        std::string name;
        std::string address;
        peerts.Recv(&name, &address);
        printf("cli name is %s, cli address is %s\n", name.c_str(), address.c_str());
        peerts.Recv(&name, &address);
        printf("cli name is %s, cli address is %s\n", name.c_str(), address.c_str());
        peerts.Recv(&name, &address);
        printf("cli name is %s, cli address is %s\n", name.c_str(), address.c_str());

        printf("svr say name:");
        fflush(stdout);
        std::cin >> name;
        printf("svr say address:");
        fflush(stdout);
        std::cin >> address;

        CliInfo svrinfo;
        //不安全的原因在于 无法掌控name.c_str()大小
        strcpy(svrinfo.Name_, name.c_str());
        strcpy(svrinfo.Address_, address.c_str());
        peerts.Send(svrinfo);
    }
    ts.Close();
    return 0;
}
