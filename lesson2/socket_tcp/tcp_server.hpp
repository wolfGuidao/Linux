#pragma once
#include <functional>
#include "tcp_socket.hpp"

typedef std::function<void (const std::string& req, std::string* resp)> Handler;

class TcpServer 
{
  public:
    TcpServer(const std::string ip,uint16_t port)
      :ip_(ip)
       ,port_(port)
    {}

    bool Start(Handler handler)
    {
      //1.创建socket
      CHECK_RET(listen_sock_.Socket());

      //2.绑定端口号
      CHECK_RET(listen_sock_.Bind(ip_,port_));

      //3.进行监听
      CHECK_RET(listen_sock_.Listen());
      
      //4.进入死循环处理事件
      //
      while(1)
      {
        //5.进行accept
        TcpSocket new_sock;
        std::string ip;
        uint16_t port;

        if(!listen_sock_.Accept(&new_sock,&ip,&port))
        {
          continue;
        }

        printf("[client %s : %d ]connect!",ip.c_str(),port);

        //6.进行循环读取
        while(1)
        {
          //7.读取请求
          std::string req;
          int ret = new_sock.Recv(&req);
          if(ret < 0)
          {
            printf("[client %s : %d]disconnect!\n",ip.c_str(),port);
            continue;
          }
          if(ret == 0)
          {
            printf("[client %s : %d]关闭了链接！\n",ip.c_str(),port);
            new_sock.Close();
            break;
          }

          printf("client say : [%s : %d]\n",ip.c_str(),port);

          //8.计算响应
          std::string resp;
          handler(req,&resp);

          //9.写回响应
          new_sock.Send(resp);

          printf("[%s:%d] req: %s, resp: %s\n", ip.c_str(), port,
              req.c_str(), resp.c_str());
          
        }
      }
      return true;
    }

  private:
    TcpSocket listen_sock_;

    std::string ip_;
    uint16_t port_;
};
