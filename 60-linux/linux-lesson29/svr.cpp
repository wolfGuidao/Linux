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
    
    //想使用TcpServer当中的收发数据接口，并且使用成员变量保存新创建(接收和发送数据)出来的socket
    while(1)
    {
        TcpServer peerts;
        if(!ts.Accept(peerts))
        {
            return 0;
        }

        std::string buf;
        peerts.Recv(buf);
        printf("cli say:%s\n", buf.c_str());

        printf("svr say:");
        fflush(stdout);
        std::cin >> buf;

        peerts.Send(buf);
    }
    ts.Close();
    return 0;
}
