#pragma once
#include <functional>
#include "tcp_socket.hpp"
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>

class Poll
{
  public:
    Poll()
      :fds_(nullptr)
       ,fds_size_(15)
       ,nfds_(0)
       ,timeout_(1000)
  {
    fds_=new pollfd[fds_size_];

  }

    bool Add(TcpSocket& sock)
    {
      if(nfds_== fds_size_)
      {//需要扩容
        pollfd* new_fds = new pollfd[2 * fds_size_];

        memcpy(new_fds,fds_,fds_size_ * sizeof(pollfd));
        fds_size_ *= 2;

        delete[] fds_;
        fds_ = new_fds;
      }

      fds_[nfds_].fd = sock.GetFd();
      fds_[nfds_].events = POLLIN;
      ++nfds_;
      return true;
    }

    bool Del(TcpSocket& sock)
    {
      int del_fd = sock.GetFd();

      for(size_t i = 0;i < nfds_;++i)
      {//在fds_内才移除
        if(fds_[i].fd == del_fd)
        {
          fds_[i] = fds_[--nfds_];
          break;
        }
      }
      return true;

    }

    bool Wait(std::vector<TcpSocket>& list,int timeout = 1000)
    {
      timeout_=timeout;

      int ret = poll(fds_,nfds_,timeout_);
      if(ret < 0)
      {
        perror("Poll Wait");
        return false;

      }
      else if(ret == 0)
      {
        std::cout<<"poll wait timeout"<<std::endl;
        return false;

      }

      for(nfds_t i = 0;i < nfds_;++i)
      {
        if(fds_[i].revents == POLLIN)
        {
          TcpSocket sock(fds_[i].fd);
          list.push_back(sock);
        }

      }
      return true;
    }

    int GetNfds()
    {
      return nfds_;

    }

    const pollfd* GetFds()
    {
      return fds_;
    }

  private:
    pollfd* fds_;
    nfds_t fds_size_;
    nfds_t nfds_;
    int timeout_;
};


typedef std::function<void(const std::string& req,std::string* resp)> Handler;

class TcpPollServer
{
  public:
    TcpPollServer(const std::string& ip,const uint16_t port)
      :ip(ip)
       ,port(port)
    {}

    ~TcpPollServer()
    {
      _sock.Close();

    }

    bool Start(Handler handler){

      if(!_sock.Socket())
        return false;

      if(!_sock.Bind(ip,port))
        return false;

      if(!_sock.Listen())
        return false;

      Poll poll;
      poll.Add(_sock);

      while(1)
      {
        std::vector<TcpSocket> list;

        if(!poll.Wait(list))
          continue;

        for(size_t i = 0;i < list.size();++i)
        {
          if(list[i].GetFd() == _sock.GetFd())
          {
            TcpSocket NewSock;
            if(!_sock.Accept(&NewSock,NULL,NULL))
              continue;

            printf("客户端已连接！~\n");
            poll.Add(NewSock);
          }
          else
          {
            std::string req;
            int n = list[i].Recv(&req);
            if(n == 0)
            {
              poll.Del(list[i]);
              list[i].Close();
              continue;

            }
            else
            {
             std::cout<<"客户端发送："<<req<<std::endl;
              std::string resp;
              handler(req,&resp);
              list[i].Send(resp);
            }
          }
        }
      }
      return true;
    }
  private:
    TcpSocket _sock;
    std::string ip;
    uint16_t port;
};
