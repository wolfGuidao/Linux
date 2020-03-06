#include "udpsvr.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("./cli [ip] [port]");
        return 0;
    }

    //服务端的ip和端口
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    UdpSvr us;
    if(!us.CreatSock())
    {
        return 0;
    }
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    while(1)
    {
        std::string buf;
        printf("client say:");
        fflush(stdout);
        std::cin>>buf;

        //数据目标的地址信息
        us.Send(buf, &dest_addr);

        us.Recv(buf, &dest_addr);

        printf("server say: [%s]\n", buf.c_str());
    }
    us.Close();
    return 0;
}
