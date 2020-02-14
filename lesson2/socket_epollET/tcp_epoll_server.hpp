#pragma once
#include <vector>
#include <functional>
#include <sys/epoll.h>
#include "tcp_socket.hpp"

typedef std::function<void (const std::string&, std::string* resp)> Handler;


//如果需要设置为非阻塞方式，需要在tcp_socket.hpp中提供非阻塞方式的recv和send接口
class Epoll 
{
  public:
    Epoll() 
    {
      epoll_fd_ = epoll_create(10);
    }

    ~Epoll() 
    {
      close(epoll_fd_);
    }

    bool Add(TcpSocket& sock, bool epoll_et = false) const 
    {
      int fd = sock.GetFd();
      printf("[Epoll Add] fd = %d\n", fd);

      epoll_event ev;
      ev.data.fd = fd;
      if (epoll_et)//如果为true，说明要设为非阻塞方式 
      {
        ev.events = EPOLLIN | EPOLLET;
      } 
      else 
      {
        ev.events = EPOLLIN;
      }

      int ret = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
      if (ret < 0) 
      {
        perror("epoll_ctl ADD");
        return false;
      }

      return true;
    }

    bool Del(TcpSocket& sock) const 
    {
      int fd = sock.GetFd();
      printf("[Epoll Del] fd = %d\n", fd);

      int ret = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
      if (ret < 0) 
      {
        perror("epoll_ctl DEL");
        return false;
      }

      return true;
    }

    bool Wait(std::vector<TcpSocket>* output) const 
    {
      output->clear();

      epoll_event events[1000];
      int nfds = epoll_wait(epoll_fd_, events, sizeof(events) / sizeof(events[0]), -1);
      if (nfds < 0) 
      {
        perror("epoll_wait");
        return false;
      }

      // [注意!] 此处必须是循环到 nfds, 不能多循环
      for (int i = 0; i < nfds; ++i) 
      {
        TcpSocket sock(events[i].data.fd);
        output->push_back(sock);
      }

      return true;
    }

  private:
    int epoll_fd_;
};

class TcpEpollServer 
{
  public:
    TcpEpollServer(const std::string& ip, uint16_t port) : ip_(ip), port_(port) 
    {}

    bool Start(Handler handler) 
    {
      // 1. 创建 socket
      TcpSocket listen_sock;
      CHECK_RET(listen_sock.Socket());

      // 2. 绑定
      CHECK_RET(listen_sock.Bind(ip_, port_));

      // 3. 监听
      CHECK_RET(listen_sock.Listen(5));

      // 4. 创建 Epoll 对象, 并将 listen_sock 加入进去
      Epoll epoll;
      epoll.Add(listen_sock);

      // 5. 进入事件循环
      for (;;) 
      {
        // 6. 进行 epoll_wait
        std::vector<TcpSocket> output;
        if (!epoll.Wait(&output)) 
        {
          continue;
        }

        // 7. 根据就绪的文件描述符的种类决定如何处理
        for (size_t i = 0; i < output.size(); ++i) 
        {
          if (output[i].GetFd() == listen_sock.GetFd()) 
          {
            // 如果是 listen_sock, 就调用 accept
            TcpSocket new_sock;
            listen_sock.Accept(&new_sock);
            epoll.Add(new_sock, true);
          } 
          else 
          {
            // 如果是 new_sock, 就进行一次读写
            std::string req, resp;
            bool ret = output[i].RecvNoBlock(&req);
            if (!ret) 
            {
              // [注意!!] 需要把不用的 socket 关闭
              // 先后顺序别搞反. 不过在 epoll 删除的时候其实就已经关闭 socket 了
              epoll.Del(output[i]);
              output[i].Close();
              continue;
            }

            handler(req, &resp);

            output[i].SendNoBlock(resp);
            printf("[client %d] req: %s, resp: %s\n", output[i].GetFd(),
                req.c_str(), resp.c_str());
          } // end for
        } // end for (;;)
      }
      return true;
    }

  private:
    std::string ip_;
    uint16_t port_;
};
