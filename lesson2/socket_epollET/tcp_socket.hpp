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

    bool SetNoBlock() 
    {
      int fl = fcntl(fd_, F_GETFL);
      if (fl < 0) 
      {
        perror("fcntl F_GETFL");
        return false;
      }

      int ret = fcntl(fd_, F_SETFL, fl | O_NONBLOCK);
      if (ret < 0) 
      {
        perror("fcntl F_SETFL");
        return false;
      }
      return true;

    }

    bool RecvNoBlock(std::string* buf) const 
    {
      // 对于非阻塞 IO 读数据, 如果 TCP 接受缓冲区为空, 就会返回错误
      // 错误码为 EAGAIN 或者 EWOULDBLOCK, 这种情况也是意料之中, 需要重试
      // 如果当前读到的数据长度小于尝试读的缓冲区的长度, 就退出循环
      // 这种写法其实不算特别严谨(没有考虑粘包问题)
      buf->clear();
      char tmp[1024 * 10] = {0};

      for (;;) 
      {
        ssize_t read_size = recv(fd_, tmp, sizeof(tmp) - 1, 0);
        if (read_size < 0) 
        {
          if (errno == EWOULDBLOCK || errno == EAGAIN) 
          {
            continue;
          }
          perror("recv");
          return false;
        }

        if (read_size == 0) 
        {
          // 对端关闭, 返回 false
          return false;
        }

        tmp[read_size] = '\0';
        *buf += tmp;

        if (read_size < (ssize_t)sizeof(tmp) - 1) 
        {
          break;
        }
      }
      return true;
    }

    bool SendNoBlock(const std::string& buf) const 
    {
      // 对于非阻塞 IO 的写入, 如果 TCP 的发送缓冲区已经满了, 就会出现出错的情况
      // // 此时的错误号是 EAGAIN 或者 EWOULDBLOCK. 这种情况下不应放弃治疗
      // // 而要进行�
      // 试
      ssize_t cur_pos = 0; // 记录当前写到的位置
      ssize_t left_size = buf.size();

      for (;;) 
      {
        ssize_t write_size = send(fd_, buf.data() + cur_pos, left_size, 0);
        if (write_size < 0) 
        {
          if (errno == EAGAIN || errno == EWOULDBLOCK) 
          {
            // 重试写入
            continue;
          }
          return false;
        }

        cur_pos += write_size;
        left_size -= write_size;

        // 这个条件说明写完需要的数据了
        if (left_size <= 0) 
        {
          break;
        }
      }

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

    int Recv(std::string* buf)
    {
      buf->clear();
      char temp[1024 * 10]= {0};

      ssize_t  read_size = recv(fd_,temp,sizeof(temp),0);
      if(read_size < 0)
      {
        perror("recv is error");
        return -1;
      }
      if(read_size == 0)
      {
        return 0;
      }

      buf->assign(temp,read_size);
      return 1;
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
      return true;
    }

    int GetFd()
    {
      return fd_;
    }
  private:
    int fd_;
};
