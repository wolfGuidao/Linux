#pragma once
#include "tcp_socket.hpp"
#include <functional>
#include <signal.h>

typedef std::function<void (const std::string& req,std::string* resp)> Handler;

class TcpProcessServer
{
  public:
    TcpProcessServer(const std::string& ip,uint16_t port)
      :ip_(ip)
       ,port_(port)
  {
    //对于多进程版本服务器，在请求到来时生成子进程处理请求，父进程继续accept。如果父进程不等待
    //子进程结束，子进程将成为僵尸进程（zombie）从而占用系统资源。如果父进程等待子进程结束
    //将增加父进程的负担，影响服务器进程的并发性能。在Linux下可以简单地将 SIGCHLD信号的操作设为SIG_IGN。
    //将SIGCHLD（子进程结束自动像父进程发送的信号）此信号设为忽略SIG_IGN
    signal(SIGCHLD,SIG_IGN);
  }

    void ProcessConnect(TcpSocket& new_sock,const std::string& ip,uint16_t port,Handler handler)
    {
      int ret = fork();
      if(ret > 0)
      {
        //father
        //父进程不需要做额外操作，只需要循环accept即可，就直接返回
        //此处如果采用wait操作，那么就达不到采用多进程的目的，采用多进程为了可以同时处理多个用户
        //如果在此处wait，那么父进程就会等待子进程处理客户端请求结束，特别耗费时间
        new_sock.Close();
        return;
      }
      else if(ret == 0)
      {
        //child
        //处理具体的链接过程即根据请求计算响应
        while(1)
        {
          std::string req;
          bool ret = new_sock.Recv(&req);
          if(!ret)
          {
            printf("[client %s : %d] disconnect!\n",ip.c_str(),port);
            exit(0);
          }

          std::string resp;
          handler(req,&resp);

          new_sock.Send(resp);

          printf("[client %s : %d ] req : %s, resp : %s\n",ip.c_str(),port,req.c_str(),resp.c_str());

        }
      }
      else
      {
        perror("fork is error");

      }
    }

    bool Start(Handler handler)
    {
      //1. 创建 socket;
      CHECK_RET(listen_sock_.Socket());

      // 2. 绑定端口号
      CHECK_RET(listen_sock_.Bind(ip_, port_));

      // 3. 进行监听
      CHECK_RET(listen_sock_.Listen(5));

      // 4. 进入事件循环
      for (;;) 
      {
        // 5. 进行 accept
        TcpSocket new_sock;
        std::string ip;
        uint16_t port = 0;
        if (!listen_sock_.Accept(&new_sock, &ip, &port)) 
        {
          continue;
        }

        printf("[client %s:%d] connect!\n", ip.c_str(), port);
        ProcessConnect(new_sock, ip, port, handler);
      }
      return true;
    }
  private:
    TcpSocket listen_sock_;
    std::string ip_;
    uint16_t port_;
};
