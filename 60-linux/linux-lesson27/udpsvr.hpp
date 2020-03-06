#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

class UdpSvr
{

    public:
        UdpSvr()
        {
            Sock_ = -1;
        }
        ~UdpSvr()
        {}
        //创建套接字
        bool CreatSock()
        {
            Sock_ = socket(AF_INET, SOCK_DGRAM, 17);
            if(Sock_ < 0)
            {
                perror("socket");
                return false;
            }
            return true;
        }

        //绑定地址信息
        bool Bind(std::string& ip, uint16_t port)
        {
            //ip + port
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            //主机字节序变成网络字节序
            addr.sin_port = htons(port);//两个字节，牵扯到大小端的问题了
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            
            int ret = bind(Sock_, (struct sockaddr*)&addr, sizeof(addr));
            if(ret < 0)
            {
                perror("bind");
                return false;
            }
            return true;
        }

        //发送数据
        bool Send(std::string& buf, struct sockaddr_in* destaddr)
        {
            //在调用sizeof计算地址信息长度的时候，不能使用sizeof(destaddr),是由于计算出来是指针的大小
            int sendsize = sendto(Sock_, buf.c_str(), buf.size(), 0, (struct sockaddr*)destaddr, sizeof(struct sockaddr_in));
            if(sendsize < 0)
            {
                perror("sendto");
                return false;
            }
            return true;
        }

        bool Recv(std::string& buf, struct sockaddr_in* srcaddr)
        {
            char tmp[1024] = {0};

            socklen_t socklen = sizeof(struct sockaddr_in);
            int recvsize = recvfrom(Sock_, tmp, sizeof(tmp) - 1, 0, (struct sockaddr*)srcaddr, &socklen);
            if(recvsize < 0)
            {
                perror("recvfrom");
                return false;
            }

            buf.assign(tmp, recvsize);
            return true;
        }

        void Close()
        {
           close(Sock_);
           Sock_ = -1;
        }
    private:
        int Sock_;
};
