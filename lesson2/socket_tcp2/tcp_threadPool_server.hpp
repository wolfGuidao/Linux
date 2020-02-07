#pragma once
#include"tcp_socket.hpp"
#include<pthread.h>
#include"threadpool.hpp"
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
#include <functional>

typedef std::function<void (const std::string& req,std::string* resp)> Handler;

struct Arg
{
  TcpSocket sock_;
  std::string _ip;
  uint16_t _port;
  Handler _handler;
};

class MyTask:public Task
{
  public:
    MyTask(void* arg)
      :arg_(arg)
    {}

    virtual void Run()override
    {
      Arg *arg = (Arg*)arg_;
      TcpSocket newsock_ = arg->sock_;
      std::string ArgIp = arg->_ip;
      int ArgPort = arg->_port;
      Handler ArgHandler = arg->_handler;

      while(1)
      {
        std::string msg;
        int n = newsock_.Recv(&msg);
        if(n < 0)
        {
          continue;

        }
        else if(n == 0)
        {
          printf("[%s:%d]客户端已经关闭！\n",ArgIp.c_str(),ArgPort);
          break;

        }

        printf("[%s:%d]客户端输入：%s\n",ArgIp.c_str(),ArgPort,msg.c_str());

        std::string resp;
        ArgHandler(msg,&resp);

        if(newsock_.Send(resp) == false)
          continue;
      }
      newsock_.Close();
      delete arg;
    }

  private:
    void* arg_;
};


class TcpThreadPoolServer
{
  public:
    TcpThreadPoolServer(const std::string ip,uint16_t port)
      :pool(10)
       ,ip_(ip)
       ,port_(port)
    {}

    ~TcpThreadPoolServer()
    {
      sock_.Close();

    }

    bool Start(Handler handler)
    {
      if(sock_.Socket() == false)
        return false;
      
      cout<<__LINE__<<endl;
      if(sock_.Bind(ip_,port_) == false)
        return false;
      cout<<__LINE__<<endl;

      if(sock_.Listen() == false)
        return false;

      while(1)
      {
        Arg *NewArg = new Arg;
        NewArg->_handler = handler;
        if(sock_.Accept(&(NewArg->sock_),&NewArg->_ip,&NewArg->_port) == false)
          continue;

        printf("[%s:%d]客户端已连接！\n",NewArg->_ip.c_str(),NewArg->_port);
        pool.AddTask(new MyTask((void*)NewArg));
      }
    }

  private:
    TcpSocket sock_;
    ThreadPool pool;
    std::string ip_;
    uint16_t port_;
};

