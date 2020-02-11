#pragma once
#include <functional>
#include "udp_socket.hpp"

typedef std::function<void(const std::string&, std::string* resp)> Handler;

class UdpServer
{
  public:
    UdpServer()
    {
      //在构造函数内部创建sock
      assert(sock_.Socket());
    }

    ~UdpServer()
    {
      sock_.Close();
    }

    bool Start(const std::string& ip, uint16_t port,Handler handler)
    {
      printf("server is start!\n");

      //绑定端口号
      bool ret = sock_.Bind(ip,port);
      if(!ret)
      {
        return false;
      }

      //进入死循环，循环处理客户的请求，并计算响应返回结果
      while(1)
      {
        //读取客户端的请求
        std::string req;
        std::string remote_ip;
        uint16_t remote_port;

        bool ret = sock_.RecvFrom(&req,&remote_ip,&remote_port);
        if(!ret )
        {
          //如果读取失败尝试循环继续读取，不能直接退出
          continue;
        }
        
        //根据请求经过Handler计算响应，并返回结果
        std::string resp;
        handler(req,&resp);

        ret = sock_.SendTo(resp,remote_ip,remote_port);
        printf("[%s : %d] req: %s, resp: %s\n",remote_ip.c_str(),remote_port,req.c_str(),resp.c_str());
      }

      sock_.Close();
      return true;
    }

  private:
    UdpSocket sock_;
};
