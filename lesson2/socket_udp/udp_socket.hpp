#pragma once
#include <cstdio>
#include <cstring>
#include <string>
#include <cassert>
#include <cstdlib>

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

class UdpSocket
{
  public:
    UdpSocket()
      :fd_(-1)
    {}

    bool Socket()
    {
      //创建socket操作句柄fd
      fd_ = socket(AF_INET,SOCK_DGRAM,0);
      if(fd_ < 0)
      {
        perror("socket error");
        return false;
      }

      return true;
    }

    //关闭套节字
    bool Close()
    {
      close(fd_);
      return true;
    }

    //绑定端口、IP
    bool Bind(const std::string& ip,uint16_t port)
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());//把点分十进制类型的ip字符串转化为32位的IP地址
      addr.sin_port = htons(port);//把主机字节序转化为网络字节序

      int ret  = bind(fd_,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("bind is error");
        return false;
      }

      return true;

    }

    //接收数据
    //出参
    bool RecvFrom(std::string* buf,std::string* ip = nullptr,uint16_t* port = nullptr)
    {
      //出参，由函数调用时填充
      char temp[1024 * 10] = {0};
      sockaddr_in peer;
      socklen_t len = sizeof(peer);

      ssize_t read_size = recvfrom(fd_,temp,sizeof(temp) -1,0,(sockaddr*)&peer,&len);
      if(read_size < 0)
      {
        perror("recvfrom is error");
        return false;
      }

      buf->assign(temp,read_size);

      if(ip != nullptr)
      {
        *ip = inet_ntoa(peer.sin_addr);
      }
      if(port != nullptr)
      {
        *port = ntohs(peer.sin_port);
      }

      return true;
    }

    //发送数据
    bool SendTo(const std::string& buf,const std::string& ip,uint16_t port)
    {
      sockaddr_in addr;

      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);

      ssize_t write_size = sendto(fd_,buf.data(),buf.size(),0,(sockaddr*)&addr,sizeof(addr));
      if(write_size < 0)
      {
        perror("sendto is error");
        return false;
      }

      return true;
    }
  private:
    int fd_;
};
