#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

#define CHECK_RET(exp) if(!(exp)){\
    return false;\
}

class TcpSocket
{
  public:
    TcpSocket()
      :fd_(-1)
    {}

    TcpSocket(int fd)
      :fd_(fd)
    {}

    bool Socket()
    {
      fd_ = socket(AF_INET,SOCK_STREAM,0);
      if(fd_ < 0)
      {
        perror("socket is error");
        return false;
      }

      printf("socket fd is successful! fd = %d\n",fd_);
      return true;
    }

    bool Close()
    {
      printf("close fd! fd = %d ",fd_);
      close(fd_);
      return true;
    }

    bool Bind(const std::string& ip,uint16_t port)
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);

      int ret = bind(fd_,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("bind is error");
        return false;
      }

      return true;
    }

    bool Listen(int num = 5)
    {
      int ret = listen(fd_,num);
      if(ret < 0)
      {
        perror("listen is error");
        return false;
      }

      return true;
    }

    bool Accept(TcpSocket* peer,std::string* ip = nullptr,uint16_t* port = nullptr)
    {
      sockaddr_in peer_addr;
      socklen_t len = sizeof(peer_addr);

      int new_sock = accept(fd_,(sockaddr*)&peer_addr,&len);
      if(new_sock < 0)
      {
        perror("accept is error");
        return false;
      }

      printf("accept new fd! fd = %d\n",new_sock);

      peer->fd_ = new_sock;

      if(ip != nullptr)
      {
        *ip = inet_ntoa(peer_addr.sin_addr); 
      }
      if(port != nullptr)
      {
        *port = ntohs(peer_addr.sin_port);
      }

      return true;
    }

    bool Recv(std::string* buf)
    {
      buf->clear();
      char temp[1024 * 10]= {0};
      
      ssize_t  read_size = recv(fd_,temp,sizeof(temp),0);
      if(read_size < 0)
      {
        perror("recv is error");
        return false;
      }
      if(read_size == 0)
      {
        return false;
      }

      buf->assign(temp,read_size);
      return true;
    }
    
    bool Send(const std::string& buf)
    {
      ssize_t write_size = send(fd_,buf.data(),buf.size(),0);
      if(write_size < 0)
      {
        perror("send is error");
        return false;
      }
      return true;
    }

    bool Connect(const std::string& ip,uint16_t port)
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);

      int ret = connect(fd_,(sockaddr*)&addr,sizeof(addr));
      if(ret < 0)
      {
        perror("connect is error");
        return false;
      }
    }
  private:
    int fd_;
};
