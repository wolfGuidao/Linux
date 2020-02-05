#pragma once
#include <functional>
#include <pthread.h>
#include "tcp_socket.hpp"

typedef std::function<void (const std::string&, std::string*)> Handler;

struct ThreadArg 
{
  TcpSocket new_sock;
  std::string ip;
  uint16_t port;
  Handler handler;
};

class TcpThreadServer 
{
  public:
    TcpThreadServer(const std::string& ip, uint16_t port) 
      : ip_(ip), port_(port) 
    {

    } 

    bool Start(Handler handler) 
    {
      // 1. 创建 socket;
      CHECK_RET(listen_sock_.Socket());

      // // 2. 绑定端口号
      CHECK_RET(listen_sock_.Bind(ip_, port_));

      // // 3. 进行监听
      CHECK_RET(listen_sock_.Listen(5));

      // 4. 进入循环
      for (;;) 
      {
        // 5. 进行 accept
        ThreadArg* arg = new ThreadArg();
        arg->handler = handler;

        bool ret = listen_sock_.Accept(&arg->new_sock, &arg->ip, &arg->port);
        if (!ret) 
        {
          continue;
        }

        printf("[client %s:%d] connect\n", arg->ip.c_str(), arg->port);

        // 6. 创建新的线程完成具体操作
        pthread_t tid;
        pthread_create(&tid, NULL, ThreadEntry, arg);
        pthread_detach(tid);
      }
      return true;
    }

    static void* ThreadEntry(void* arg) 
    {
      ThreadArg* p = reinterpret_cast<ThreadArg*>(arg);
      PthreadConnect(p);
      // 一定要记得释放内存!!! 也要记得关闭文件描述符
      p->new_sock.Close();
      delete p;
      return NULL;
    }

    static void PthreadConnect(ThreadArg* arg) 
    {
      // 1. 循环进行读写
      for (;;) 
      {
        std::string req;

        // 2. 读取请求
        bool ret = arg->new_sock.Recv(&req);
        if (!ret) 
        {
          printf("[client %s:%d] disconnected!\n", arg->ip.c_str(), arg->port);
          break;
        }

        std::string resp;
        // 3. 根据请求计算响应
        arg->handler(req, &resp);

        // 4. 发送响应
        arg->new_sock.Send(resp);
        printf("[client %s:%d] req: %s, resp: %s\n", arg->ip.c_str(),
            arg->port, req.c_str(), resp.c_str());
      }
    }

  private:
    TcpSocket listen_sock_;
    std::string ip_;
    uint16_t port_;
};
