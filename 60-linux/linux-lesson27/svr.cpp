#include "udpsvr.hpp"

// ip port
// 命令行参数的方式去获取这两个信息
// ./svr ip port
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./svr [ip] [port]");
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    UdpSvr us;
    if(!us.CreatSock())
    {
        return 0;
    }

    if(!us.Bind(ip, port))
    {
        return 0;
    }
    while(1)
    {
        std::string buf;
        //数据来源的地址信息
        struct sockaddr_in peer_addr;
        us.Recv(buf, &peer_addr);

        printf("client say: [%s]\n", buf.c_str());
        printf("server say:");
        fflush(stdout);

        std::cin >> buf;
        //发送
        us.Send(buf, &peer_addr);
    }
    us.Close();

    return 0;
}
